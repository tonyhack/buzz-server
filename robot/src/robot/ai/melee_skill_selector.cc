#include "robot/ai/melee_skill_selector.h"

#include "robot/logging.h"
#include "robot/config.h"
#include "robot/actor.h"
#include "protocol/vocation_types.h"

namespace robot {

MeleeSkillSelector::MeleeSkillSelector()
{
}

MeleeSkillSelector::~MeleeSkillSelector()
{
}

SkillSelector *MeleeSkillSelector::clone()
{
    return new MeleeSkillSelector();
}

const SkillCell *MeleeSkillSelector::getSkill(Actor *actor)
{
    int vocation = actor->getVocation();

    if (entity::VocationType::AXE == vocation) {
        return Config::getInstance()->getSkillCell(
            Config::getInstance()->axe_melee_skill_id_, 1);

    } else if (entity::VocationType::SHOOT == vocation) {
        return Config::getInstance()->getSkillCell(
            Config::getInstance()->shoot_melee_skill_id_, 1);

    } else if (entity::VocationType::MAGIC == vocation) {
        return Config::getInstance()->getSkillCell(
            Config::getInstance()->magic_melee_skill_id_, 1);

    } else {
        return NULL;
    }
}

} // namespace robot

