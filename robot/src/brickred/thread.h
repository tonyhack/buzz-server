#ifndef BRICKRED_THREAD_H
#define BRICKRED_THREAD_H

#include <cstddef>

#include <brickred/class_util.h>
#include <brickred/function.h>
#include <brickred/unique_ptr.h>

namespace brickred {

class Thread {
public:
    typedef Function<void ()> ThreadFunc;

    Thread();
    ~Thread();

    void start(ThreadFunc thread_func);
    bool joinable();
    void join();
    void detach();

private:
    BRICKRED_NONCOPYABLE(Thread)

    class Impl;
    UniquePtr<Impl> pimpl_;
};

class ThreadGuard {
public:
    explicit ThreadGuard(Thread &t) : thread_(t) {}

    ~ThreadGuard()
    {
        if (thread_.joinable()) {
            thread_.join();
        }
    }

private:
    BRICKRED_NONCOPYABLE(ThreadGuard)

    Thread &thread_;
};

} // namespace brickred

#endif
