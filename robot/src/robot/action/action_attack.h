#ifndef BUZZ_ROBOT_ROBOT_ACTION_ACTION_ATTACK_H
#define BUZZ_ROBOT_ROBOT_ACTION_ACTION_ATTACK_H

#include <stdint.h> 
#include <cstddef>

#include "robot/action/action.h"

namespace robot {

class TargetSelector;
class SkillSelector;
class ActionApproachTarget;
class SkillCell;

class ActionAttack : public Action {
public:
    ActionAttack(TargetSelector *target_selector,
                 SkillSelector *skill_selector);
    virtual ~ActionAttack();
    virtual Action *clone();

    virtual void start();
    virtual void reset();

private:
    void attack();
    void onAttackTimer(int64_t timer_id);
    void onApproachComplete(bool success);

    TargetSelector *target_selector_;
    SkillSelector *skill_selector_;
    ActionApproachTarget *action_approach_target_;
    int64_t target_id_;
    int64_t timer_id_;
};

} // namespace robot

#endif

