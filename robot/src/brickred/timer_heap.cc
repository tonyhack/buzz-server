#include <brickred/timer_heap.h>

#include <cstddef>
#include <algorithm>
#include <vector>
#include <ext/hash_map>

#include <brickred/base_logger.h>
#include <brickred/io_service.h>
#include <brickred/timestamp.h>

namespace brickred {

class TimerIdAllocator {
public:
    TimerIdAllocator() : value_(0) {}
    ~TimerIdAllocator() {}

    int64_t getId()
    {
        if (++value_ < 0) {
            value_ = 0;
        }
        return value_;
    }

private:
    int64_t value_;
};

class Timer {
public:
    typedef TimerHeap::TimerId TimerId;
    typedef TimerHeap::TimerCallback TimerCallback;

    explicit Timer(TimerId id, const Timestamp &timestamp, int timeout,
                   const TimerCallback &timer_cb, int call_times);
    ~Timer() {}

    TimerId getId() const { return id_; }
    const Timestamp &getTimestamp() const { return timestamp_; }
    Timestamp &getTimestamp() { return timestamp_; }
    int getTimeout() const { return timeout_; }
    int getHeapPos() const { return heap_pos_; }
    void setHeapPos(int heap_pos) { heap_pos_ = heap_pos; }
    int getCallTimes() const { return call_times_; }
    void decCallTimes() { --call_times_; }
    TimerCallback getCallback() const { return timer_cb_; }

private:
    TimerId id_;
    Timestamp timestamp_;
    int timeout_;
    TimerCallback timer_cb_;
    int call_times_;
    int heap_pos_;
};

///////////////////////////////////////////////////////////////////////////////
Timer::Timer(TimerId id, const Timestamp &timestamp, int timeout,
             const TimerCallback &timer_cb, int call_times) :
    id_(id), timestamp_(timestamp), timeout_(timeout),
    timer_cb_(timer_cb), call_times_(call_times),
    heap_pos_(-1)
{
}

///////////////////////////////////////////////////////////////////////////////
class TimerHeap::Impl {
public:
    typedef TimerHeap::TimerId TimerId;
    typedef TimerHeap::TimerCallback TimerCallback;
    typedef __gnu_cxx::hash_map<TimerId, Timer *> TimerMap;
    typedef std::vector<Timer *> TimerVector;

    Impl();
    ~Impl();

    int64_t getNextTimeoutMillisecond(const Timestamp &now) const;
    TimerId addTimer(const Timestamp &now, int timeout_ms,
                     const TimerCallback &timer_cb,
                     int call_times = -1);
    void removeTimer(TimerId timer_id);
    void checkTimeout(const Timestamp &now);

public:
    void minHeapReserve();
    void minHeapInsert(Timer *timer);
    void minHeapErase(Timer *timer);
    Timer *minHeapTop() const;

private:
    TimerIdAllocator timer_id_allocator_;
    TimerMap timers_;
    TimerVector timer_min_heap_;
};

///////////////////////////////////////////////////////////////////////////////
TimerHeap::Impl::Impl()
{
    timer_min_heap_.push_back(NULL);
}

TimerHeap::Impl::~Impl()
{
    for (TimerMap::iterator iter = timers_.begin();
         iter != timers_.end(); ++iter) {
        delete iter->second;
    }
}

int64_t TimerHeap::Impl::getNextTimeoutMillisecond(const Timestamp &now) const
{
    Timer *timer = minHeapTop();
    if (NULL == timer) {
        return -1;
    }
    return now.distanceMillisecond(timer->getTimestamp());
}

TimerHeap::Impl::TimerId TimerHeap::Impl::addTimer(const Timestamp &now,
    int timeout_ms, const TimerCallback &timer_cb, int call_times)
{
    TimerId timer_id = timer_id_allocator_.getId();
    UniquePtr<Timer> timer(new Timer(timer_id,
                                     now + timeout_ms,
                                     timeout_ms,
                                     timer_cb,
                                     call_times));

    if (timers_.find(timer_id) != timers_.end()) {
        BASE_ERROR("timer(%lu) already in timer map", timer_id);
        return -1;
    }

    // reserve min heap
    minHeapReserve();

    // insert into timer map
    timers_.insert(std::make_pair(timer_id, timer.get()));
    Timer *timer_raw = timer.release();

    // insert into timer mini heap
    minHeapInsert(timer_raw);

    return timer_id;
}

void TimerHeap::Impl::removeTimer(TimerId timer_id)
{
    TimerMap::iterator iter = timers_.find(timer_id);
    if (timers_.end() == iter) {
        return;
    }

    Timer *timer = iter->second;

    // remove from timer mini heap
    minHeapErase(timer);

    // remove from timer map
    timers_.erase(iter);
    delete timer;
}

void TimerHeap::Impl::checkTimeout(const Timestamp &now)
{
    for (;;) {
        Timer *timer = minHeapTop();
        if (NULL == timer) {
            return;
        }

        if (now < timer->getTimestamp()) {
            return;
        }

        TimerId timer_id = timer->getId();
        TimerCallback timer_cb = timer->getCallback();

        // remove from timer mini heap
        minHeapErase(timer);

        if (timer->getCallTimes() - 1 == 0) {
            // remove from timer map
            timers_.erase(timer_id);
            delete timer;
        } else {
            timer->decCallTimes();
            timer->getTimestamp() += timer->getTimeout();
            // insert into timer mini heap again
            minHeapInsert(timer);
        }

        // do callback
        timer_cb(timer_id);
    }
}

void TimerHeap::Impl::minHeapReserve()
{
    timer_min_heap_.reserve(timer_min_heap_.size() + 1);
}

void TimerHeap::Impl::minHeapInsert(Timer *timer)
{
    int cur_index = timer_min_heap_.size();
    timer_min_heap_.push_back(timer);
    timer->setHeapPos(cur_index);

    for (;;) {
        int parent_index = cur_index / 2;

        if (0 == parent_index) {
            break;
        }
        if (timer_min_heap_[parent_index]->getTimestamp() <
            timer_min_heap_[cur_index]->getTimestamp()) {
            break;
        }

        timer_min_heap_[parent_index]->setHeapPos(cur_index);
        timer_min_heap_[cur_index]->setHeapPos(parent_index);
        std::swap(timer_min_heap_[parent_index], timer_min_heap_[cur_index]);

        cur_index = parent_index;
    }
}

void TimerHeap::Impl::minHeapErase(Timer *timer)
{
    int cur_index = timer->getHeapPos();
    if (cur_index < 0) {
        return;
    }

    timer->setHeapPos(-1);
    timer_min_heap_[cur_index] = timer_min_heap_.back();
    timer_min_heap_[cur_index]->setHeapPos(cur_index);
    timer_min_heap_.pop_back();

    for (;;) {
        int child_index = cur_index * 2;

        if (child_index >= (int)timer_min_heap_.size()) {
            break;
        }

        if (child_index + 1 < (int)timer_min_heap_.size() &&
            timer_min_heap_[child_index + 1]->getTimestamp() <
            timer_min_heap_[child_index]->getTimestamp()) {
            ++child_index;
        }

        if (timer_min_heap_[cur_index]->getTimestamp() <
            timer_min_heap_[child_index]->getTimestamp()) {
            break;
        }

        timer_min_heap_[cur_index]->setHeapPos(child_index);
        timer_min_heap_[child_index]->setHeapPos(cur_index);
        std::swap(timer_min_heap_[cur_index], timer_min_heap_[child_index]);
        cur_index = child_index;
    }
}

Timer *TimerHeap::Impl::minHeapTop() const
{
    if (timer_min_heap_.size() <= 1) {
        return NULL;
    }
    return timer_min_heap_[1];
}

///////////////////////////////////////////////////////////////////////////////
TimerHeap::TimerHeap() :
    pimpl_(new Impl())
{
}

TimerHeap::~TimerHeap()
{
}

int64_t TimerHeap::getNextTimeoutMillisecond(const Timestamp &now) const
{
    return pimpl_->getNextTimeoutMillisecond(now);
}

TimerHeap::TimerId TimerHeap::addTimer(const Timestamp &now,
    int timeout_ms, const TimerCallback &timer_cb, int call_times)
{
    return pimpl_->addTimer(now, timeout_ms, timer_cb, call_times);
}

void TimerHeap::removeTimer(TimerId timer_id)
{
    pimpl_->removeTimer(timer_id);
}

void TimerHeap::checkTimeout(const Timestamp &now)
{
    pimpl_->checkTimeout(now);
}

}  // namespace brickred
