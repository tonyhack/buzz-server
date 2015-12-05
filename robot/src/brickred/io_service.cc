#include <brickred/io_service.h>

#include <stdint.h>
#include <unistd.h>

#ifdef BRICKRED_DONT_HAVE_EPOLL_CREATE1
#include <fcntl.h>
#endif

#include <sys/epoll.h>
#include <cerrno>
#include <cstring>
#include <vector>
#include <ext/hash_set>

#include <brickred/base_logger.h>
#include <brickred/exception.h>
#include <brickred/io_device.h>
#include <brickred/timer_heap.h>
#include <brickred/timestamp.h>

#define MAX_EPOLL_TIMEOUT_MSEC (35*60*1000)

namespace brickred {

class IOService::Impl {
public:
    typedef IOService::TimerId TimerId;
    typedef IOService::TimerCallback TimerCallback;
    typedef std::vector<struct epoll_event> EventVector;
    typedef __gnu_cxx::hash_set<intptr_t> IODeviceSet;

    Impl();
    ~Impl();

    bool addIODevice(IODevice *io_device);
    bool removeIODevice(IODevice *io_device);
    bool updateIODevice(IODevice *io_device);
    void loop();
    void quit();

    TimerId startTimer(int timeout_ms, const TimerCallback &timer_cb,
                       int call_times);
    void stopTimer(TimerId timer_id);

public:
    bool checkIODeviceExist(IODevice *io_device) const;

private:
    bool quit_;
    int epoll_fd_;
    EventVector events_;
    IODeviceSet removed_io_devices_;
    TimerHeap timer_heap_;
};

///////////////////////////////////////////////////////////////////////////////
IOService::Impl::Impl() : epoll_fd_(-1), events_(32)
{
#ifndef BRICKRED_DONT_HAVE_EPOLL_CREATE1
    epoll_fd_ = epoll_create1(EPOLL_CLOEXEC);
    if (-1 == epoll_fd_) {
        throw SystemErrorException(
            "create io service failed in epoll_create1");
    }
#else
    epoll_fd_ = epoll_create(32000);
    if (-1 == epoll_fd_) {
        throw SystemErrorException("create io service failed in epoll_create");
    }
    int flags = ::fcntl(epoll_fd_, F_GETFD, 0);
    if (::fcntl(epoll_fd_, F_SETFD, flags | FD_CLOEXEC) != 0) {
        throw SystemErrorException("create io service failed in fcntl");
    }
#endif
}

IOService::Impl::~Impl()
{
    if (epoll_fd_ != -1) {
        ::close(epoll_fd_);
        epoll_fd_ = -1;
    }
}

bool IOService::Impl::addIODevice(IODevice *io_device)
{
    struct epoll_event event;
    ::memset(&event, 0, sizeof(event));
    event.events = 0;
    event.data.ptr = io_device;

    if (io_device->getReadCallback()) {
        event.events |= EPOLLIN | EPOLLPRI;
    }
    if (io_device->getWriteCallback()) {
        event.events |= EPOLLOUT;
    }

    if (::epoll_ctl(epoll_fd_, EPOLL_CTL_ADD,
                    io_device->getDescriptor(), &event) != 0) {
        BASE_ERROR("epoll_ctl add %d failed: %s",
                   io_device->getDescriptor(), ::strerror(errno));
        return false;
    }

    return true;
}

bool IOService::Impl::removeIODevice(IODevice *io_device)
{
    struct epoll_event event;
    ::memset(&event, 0, sizeof(event));

    if (::epoll_ctl(epoll_fd_, EPOLL_CTL_DEL,
                    io_device->getDescriptor(), &event) != 0) {
        BASE_ERROR("epoll_ctl del %d failed: %s",
                   io_device->getDescriptor(), ::strerror(errno));
        return false;
    }

    removed_io_devices_.insert((intptr_t)io_device);

    return true;
}

bool IOService::Impl::updateIODevice(IODevice *io_device)
{
    struct epoll_event event;
    ::memset(&event, 0, sizeof(event));
    event.events = 0;
    event.data.ptr = io_device;

    if (io_device->getReadCallback()) {
        event.events |= EPOLLIN | EPOLLPRI;
    }
    if (io_device->getWriteCallback()) {
        event.events |= EPOLLOUT;
    }

    if (::epoll_ctl(epoll_fd_, EPOLL_CTL_MOD,
                    io_device->getDescriptor(), &event) != 0) {
        BASE_ERROR("epoll_ctl mod %d failed: %s",
                   io_device->getDescriptor(), ::strerror(errno));
        return false;
    }

    return true;
}

bool IOService::Impl::checkIODeviceExist(IODevice *io_device) const
{
    return removed_io_devices_.find((intptr_t)io_device) ==
           removed_io_devices_.end();
}

void IOService::Impl::loop()
{
    Timestamp now;

    while (!quit_) {
        now.setNow();

        int timer_timeout = timer_heap_.getNextTimeoutMillisecond(now);
        int event_count = ::epoll_wait(epoll_fd_, &events_[0],
            events_.size(), std::min(timer_timeout, MAX_EPOLL_TIMEOUT_MSEC));
        if (-1 == event_count) {
            if (EINTR == errno) {
                continue;
            } else {
                BASE_ERROR("epoll_wait failed: %s", ::strerror(errno));
                break;
            }
        }

        // do event callback
        for (int i = 0; i < event_count; ++i) {
            struct epoll_event *event = &events_[i];
            IODevice *io_device = (IODevice *)event->data.ptr;

            if (event->events & EPOLLOUT) {
                if (checkIODeviceExist(io_device) == false) {
                    continue;
                }
                (io_device->getWriteCallback())(io_device);
            }

#ifdef EPOLLRDHUP
            if (event->events & (EPOLLIN | EPOLLPRI | EPOLLRDHUP)) {
#else
            if (event->events & (EPOLLIN | EPOLLPRI)) {
#endif
                if (checkIODeviceExist(io_device) == false) {
                    continue;
                }
                (io_device->getReadCallback())(io_device);
            }

            if (event->events & (EPOLLERR | EPOLLHUP)) {
                if (checkIODeviceExist(io_device) == false) {
                    continue;
                }
                (io_device->getErrorCallback())(io_device);
            }
        }

        // do timer callback
        now.setNow();
        timer_heap_.checkTimeout(now);

        // clear removed io devices
        removed_io_devices_.clear();

        // resize event list
        if (event_count >= (int)events_.size()) {
            events_.resize(events_.size() * 2);
        }
    }
}

void IOService::Impl::quit()
{
    quit_ = true;
}

IOService::Impl::TimerId IOService::Impl::startTimer(int timeout_ms,
    const TimerCallback &timer_cb, int call_times)
{
    Timestamp now;
    now.setNow();

    return timer_heap_.addTimer(now, timeout_ms, timer_cb, call_times);
}

void IOService::Impl::stopTimer(TimerId timer_id)
{
    timer_heap_.removeTimer(timer_id);
}


///////////////////////////////////////////////////////////////////////////////
IOService::IOService() :
    pimpl_(new Impl())
{
}

IOService::~IOService()
{
}

bool IOService::addIODevice(IODevice *io_device)
{
    return pimpl_->addIODevice(io_device);
}

bool IOService::removeIODevice(IODevice *io_device)
{
    return pimpl_->removeIODevice(io_device);
}

bool IOService::updateIODevice(IODevice *io_device)
{
    return pimpl_->updateIODevice(io_device);
}

void IOService::loop()
{
    pimpl_->loop();
}

void IOService::quit()
{
    pimpl_->quit();
}

IOService::TimerId IOService::startTimer(int timeout_ms,
    const TimerCallback &timer_cb, int call_times)
{
    return pimpl_->startTimer(timeout_ms, timer_cb, call_times);
}

void IOService::stopTimer(TimerId timer_id)
{
    return pimpl_->stopTimer(timer_id);
}

} // namespace brickred
