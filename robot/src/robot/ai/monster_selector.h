#ifndef BUZZ_ROBOT_ROBOT_AI_MONSTER_SELECTOR_H
#define BUZZ_ROBOT_ROBOT_AI_MONSTER_SELECTOR_H

#include <stdint.h>

#include "robot/ai/target_selector.h"

namespace robot {

class MonsterSelector : public TargetSelector {
public:
    struct Policy {
        enum type {
            MIN = 0,

            NEAREST = 0,
            DEFAULT = NEAREST,

            MAX
        };
    };

    MonsterSelector(int64_t template_id,
                    Policy::type policy = Policy::DEFAULT);
    virtual ~MonsterSelector();
    virtual TargetSelector *clone();

    virtual int64_t getTarget(Actor *actor);

private:
    int64_t template_id_;
    Policy::type policy_;
};

} // namespace robot

#endif

