#ifndef BUZZ_ROBOT_ROBOT_AI_TARGET_SELECTOR_H
#define BUZZ_ROBOT_ROBOT_AI_TARGET_SELECTOR_H

#include <stdint.h>
#include <brickred/class_util.h>

namespace robot {

class Actor;

class TargetSelector {
public:
    TargetSelector();
    virtual ~TargetSelector() = 0;
    virtual TargetSelector *clone() = 0;

    virtual int64_t getTarget(Actor *actor) = 0;

private:
    BRICKRED_NONCOPYABLE(TargetSelector)
};

} // namespace robot

#endif

