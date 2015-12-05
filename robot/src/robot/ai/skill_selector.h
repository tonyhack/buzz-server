#ifndef BUZZ_ROBOT_ROBOT_AI_SKILL_SELECTOR_H
#define BUZZ_ROBOT_ROBOT_AI_SKILL_SELECTOR_H

#include <stdint.h>
#include <brickred/class_util.h>

namespace robot {

class Actor;
class SkillCell;

class SkillSelector {
public:
    SkillSelector();
    virtual ~SkillSelector() = 0;
    virtual SkillSelector *clone() = 0;

    virtual const SkillCell *getSkill(Actor *actor) = 0;

private:
    BRICKRED_NONCOPYABLE(SkillSelector)
};

} // namespace robot

#endif

