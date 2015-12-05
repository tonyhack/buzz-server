#ifndef BUZZ_ROBOT_ROBOT_AI_MELEE_SKILL_SELECTOR_H
#define BUZZ_ROBOT_ROBOT_AI_MELEE_SKILL_SELECTOR_H

#include "robot/ai/skill_selector.h"

namespace robot {

class MeleeSkillSelector : public SkillSelector {
public:
    MeleeSkillSelector();
    virtual ~MeleeSkillSelector();
    virtual SkillSelector *clone();

    virtual const SkillCell *getSkill(Actor *actor);
};

} // namespace robot

#endif

