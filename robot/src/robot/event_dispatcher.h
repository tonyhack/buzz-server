#ifndef BUZZ_ROBOT_ROBOT_EVENT_DISPATCHER_H
#define BUZZ_ROBOT_ROBOT_EVENT_DISPATCHER_H

#include <stdint.h>
#include <brickred/class_util.h>
#include <brickred/unique_ptr.h>
#include <brickred/function.h>

namespace robot {

class EventDispatcher {
public:
    typedef brickred::Function<void (const void *)>
            EventHandler;

    EventDispatcher();
    ~EventDispatcher();

    int64_t follow(int event_type, EventHandler handler);
    void unfollow(int64_t token);
    void dispatch(int event_type, const void *event);

private:
    BRICKRED_NONCOPYABLE(EventDispatcher)

    class Impl;
    brickred::UniquePtr<Impl> pimpl_;
};

} // namespace robot

#endif

