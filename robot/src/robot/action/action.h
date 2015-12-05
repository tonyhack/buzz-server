#ifndef BUZZ_ROBOT_ROBOT_ACTION_ACTION_H
#define BUZZ_ROBOT_ROBOT_ACTION_ACTION_H

#include <brickred/class_util.h>
#include <brickred/function.h>

namespace robot {

class Actor;

class Action {
public:
    typedef brickred::Function<void (bool)> CompleteCallback;

    Action();
    virtual ~Action() = 0;
    virtual Action *clone() = 0;

    void setActor(Actor *actor);
    void setCompleteCallback(CompleteCallback complete_cb);

    virtual void start() = 0;
    virtual void reset() = 0;
    virtual void suspend() { reset(); }
    virtual void resume() { start(); }

protected:
    // must be last call
    void finish(bool success = true);

    Actor *actor_;
    CompleteCallback complete_cb_;

private:
    BRICKRED_NONCOPYABLE(Action)
};

} // namespace robot

#endif

