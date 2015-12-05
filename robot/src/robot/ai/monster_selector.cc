#include "robot/ai/monster_selector.h"

#include "robot/logging.h"
#include "robot/actor.h"
#include "robot/actor_sight.h"
#include "robot/aoi_npc.h"

namespace robot {

MonsterSelector::MonsterSelector(int64_t template_id,
                                 Policy::type policy) :
    template_id_(template_id), policy_(policy)
{
}

MonsterSelector::~MonsterSelector()
{
}

TargetSelector *MonsterSelector::clone()
{
    return new MonsterSelector(template_id_, policy_);
}

int64_t MonsterSelector::getTarget(Actor *actor)
{
    if (policy_ == Policy::NEAREST) {
        const ActorSight::AoiNpcMap &aoi_npcs =
            actor->getSight().getAoiNpcs();

        int min_distance = -1;
        int64_t min_distance_target = -1;

        for (ActorSight::AoiNpcMap::const_iterator iter = aoi_npcs.begin();
             iter != aoi_npcs.end(); ++iter) {
            const AoiNpc *aoi_npc = iter->second;

            if (aoi_npc->getTemplateId() != template_id_) {
                continue;
            }

            int distance = actor->getPos().getDistance(aoi_npc->getPos());

            if (min_distance == -1 || distance < min_distance) {
                min_distance = distance;
                min_distance_target = aoi_npc->getId();
            }
        }

        return min_distance_target;

    } else {
        LOG_ERROR("invalid policy value");
        return -1;
    }
}

} // namespace robot

