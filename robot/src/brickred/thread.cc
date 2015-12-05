#include <brickred/thread.h>

#include <pthread.h>

#include <brickred/condition_variable.h>
#include <brickred/exception.h>
#include <brickred/mutex.h>

namespace brickred {

class Thread::Impl {
public:
    Impl();
    ~Impl();

    void start(ThreadFunc thread_func);
    bool joinable();
    void join();
    void detach();

public:
    static void *threadProxy(void *arg);
    void run();

private:
    pthread_t thread_handle_;
    ThreadFunc thread_func_;
    Mutex data_mutex_;
    ConditionVariable done_cond_;
    bool started_;
    bool done_;
    bool join_started_;
    bool joined_;
};

///////////////////////////////////////////////////////////////////////////////
Thread::Impl::Impl() :
    thread_handle_(0),
    started_(false), done_(false), join_started_(false), joined_(false)
{
}

Thread::Impl::~Impl()
{
    if (joinable()) {
        detach();
    }
}

void Thread::Impl::start(ThreadFunc thread_func)
{
    LockGuard lock(data_mutex_);

    if (started_) {
        throw SystemErrorException("thread has already started");
    }

    thread_func_ = thread_func;

    if (::pthread_create(&thread_handle_, NULL, &threadProxy, this) != 0) {
        throw SystemErrorException("create thread failed in pthread_create");
    }

    started_ = true;
}

bool Thread::Impl::joinable()
{
    LockGuard lock(data_mutex_);
    return started_ && !joined_;
}

void Thread::Impl::join()
{
    bool do_join = false;

    {
        LockGuard lock(data_mutex_);
        if (::pthread_equal(thread_handle_, pthread_self())) {
            throw SystemErrorException("thread try to join itself");
        }

        while (!done_) {
            done_cond_.wait(data_mutex_);
        }
        do_join = !join_started_;

        if (do_join) {
            join_started_ = true;
        } else {
            while (!joined_) {
                done_cond_.wait(data_mutex_);
            }
        }
    }

    if (do_join) {
        ::pthread_join(thread_handle_, NULL);
        LockGuard lock(data_mutex_);
        joined_ = true;
        done_cond_.notifyAll();
    }
}

void Thread::Impl::detach()
{
    LockGuard lock(data_mutex_);
    if (!join_started_) {
        ::pthread_detach(thread_handle_);
        join_started_ = true;
        joined_ = true;
    }
}

void Thread::Impl::run()
{
    thread_func_();
}

void *Thread::Impl::threadProxy(void *arg)
{
    Impl *impl = static_cast<Impl *>(arg);

    impl->run();

    LockGuard lock(impl->data_mutex_);
    impl->done_ = true;
    impl->done_cond_.notifyAll();

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
Thread::Thread() :
    pimpl_(new Impl())
{
}

Thread::~Thread()
{
}

void Thread::start(ThreadFunc thread_func)
{
    pimpl_->start(thread_func);
}

bool Thread::joinable()
{
    return pimpl_->joinable();
}

void Thread::join()
{
    pimpl_->join();
}

void Thread::detach()
{
    pimpl_->detach();
}

} // namespace brickred
