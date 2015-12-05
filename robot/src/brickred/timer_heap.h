#ifndef BRICKRED_TIMER_HEAP_H
#define BRICKRED_TIMER_HEAP_H

#include <stdint.h>

#include <brickred/class_util.h>
#include <brickred/function.h>
#include <brickred/unique_ptr.h>

namespace brickred {

class IOService;
class Timestamp;

class TimerHeap {
public:
    typedef int64_t TimerId;
    typedef Function<void (TimerId)> TimerCallback;

    TimerHeap();
    ~TimerHeap();

    int64_t getNextTimeoutMillisecond(const Timestamp &now) const;
    TimerId addTimer(const Timestamp &now, int timeout_ms,
                     const TimerCallback &timer_cb,
                     int call_times = -1);
    void removeTimer(TimerId timer_id);
    void checkTimeout(const Timestamp &now);

private:
    BRICKRED_NONCOPYABLE(TimerHeap)

    class Impl;
    UniquePtr<Impl> pimpl_;
};

}  // namespace brickred

#endif
