//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-10 11:45:57.
// File name: condition_pool.h
//
// Description:
// Define class ConditionPool.
//

#ifndef __GAME__SERVER__TASK__CONDITION__POOL__H
#define __GAME__SERVER__TASK__CONDITION__POOL__H

#include "entity/task_types.h"
#include "global/pool_template.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace task {

class Condition;
class KillMobCondition;
class SoulAppearCondition;
class TalkNpcCondition;
class CollectItemCondition;
class RefineEquipCondition;
class InlayEquipCondition;
class StrengthEquipCondition;
class SpiritFeedCondition;
class UseItemCondition;
class DivineGemCondition;
class LevelUpCondition;
class KillMobCollectCondition;
class WearEquipCondition;
class SoulEquipCondition;
class SoulFeedCondition;
class PassPlayingCondition;
class GatherCondition;
class ConstellationUpCondition;
class SummonNpcCondition;
class KillActorCondition;
class KillMobTypeCondition;
class FinishTaskTypeCondition;
class RobCondition;
class KillMobLevelCondition;
class SpendResourceCondition;
class GetResourceCondition;
class WateringCondition;
class GetVipCondition;

class ConditionPool : public global::SingletonFactory<ConditionPool> {
  friend class global::SingletonFactory<ConditionPool>;

 public:
  bool Initialize(size_t initial_number, size_t extend_number);
  void Finalize();

  Condition *Allocate(entity::TaskCondition::type type);
  void Deallocate(Condition *condition);

 private:
  ConditionPool() {}
  ~ConditionPool() {}

  global::PoolTemplate<KillMobCondition> kill_mobs_;
  global::PoolTemplate<TalkNpcCondition> talk_npcs_;
  global::PoolTemplate<SoulAppearCondition> soul_appears_;
  global::PoolTemplate<CollectItemCondition> collect_items_;
  global::PoolTemplate<RefineEquipCondition> refine_equips_;
  global::PoolTemplate<InlayEquipCondition> inlay_equips_;
  global::PoolTemplate<StrengthEquipCondition> strength_equips_;
  global::PoolTemplate<SpiritFeedCondition> spirit_feeds_;
  global::PoolTemplate<UseItemCondition> use_items_;
  global::PoolTemplate<DivineGemCondition> divine_gems_;
  global::PoolTemplate<LevelUpCondition> level_ups_;
  global::PoolTemplate<KillMobCollectCondition> kill_mob_collects_;
  global::PoolTemplate<WearEquipCondition> wear_equips_;
  global::PoolTemplate<SoulEquipCondition> soul_equips_;
  global::PoolTemplate<SoulFeedCondition> soul_feeds_;
  global::PoolTemplate<PassPlayingCondition> pass_playings_;
  global::PoolTemplate<GatherCondition> gather_;
  global::PoolTemplate<ConstellationUpCondition> constellation_ups_;
  global::PoolTemplate<SummonNpcCondition> summon_npcs_;
  global::PoolTemplate<KillActorCondition> kill_actors_;
  global::PoolTemplate<KillMobTypeCondition> kill_npc_types_;
  global::PoolTemplate<FinishTaskTypeCondition> finish_task_types_;
  global::PoolTemplate<RobCondition> robs_;
  global::PoolTemplate<KillMobLevelCondition> kill_npc_levels_;
  global::PoolTemplate<SpendResourceCondition> spend_resources_;
  global::PoolTemplate<GetResourceCondition> get_resources_;
  global::PoolTemplate<WateringCondition> waterings_;
  global::PoolTemplate<GetVipCondition> get_vips_;
};

}  // namespace task

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TASK__CONDITION__POOL__H

