//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-10 11:51:05.
// File name: condition_pool.cc
//
// Description:
// Define class ConditionPool.
//

#include "game_server/server/extension/task/condition_pool.h"

#include "game_server/server/extension/task/kill_mob_condition.h"
#include "game_server/server/extension/task/soul_appear_condition.h"
#include "game_server/server/extension/task/talk_npc_condition.h"
#include "game_server/server/extension/task/collect_item_condition.h"
#include "game_server/server/extension/task/use_item_condition.h"
#include "game_server/server/extension/task/strength_equip_condition.h"
#include "game_server/server/extension/task/refine_equip_condition.h"
#include "game_server/server/extension/task/inlay_equip_condition.h"
#include "game_server/server/extension/task/wear_equip_condition.h"
#include "game_server/server/extension/task/spirit_feed_condition.h"
#include "game_server/server/extension/task/divine_gem_condition.h"
#include "game_server/server/extension/task/level_up_condition.h"
#include "game_server/server/extension/task/kill_mob_collect_condition.h"
#include "game_server/server/extension/task/soul_feed_condition.h"
#include "game_server/server/extension/task/soul_equip_condition.h"
#include "game_server/server/extension/task/summon_npc_condition.h"
#include "game_server/server/extension/task/pass_playing_condition.h"
#include "game_server/server/extension/task/gather_condition.h"
#include "game_server/server/extension/task/constellation_up.h"
#include "game_server/server/extension/task/kill_actor_condition.h"
#include "game_server/server/extension/task/kill_mob_type_condition.h"
#include "game_server/server/extension/task/rob_condition.h"
#include "game_server/server/extension/task/finish_task_type_condition.h"
#include "game_server/server/extension/task/kill_mob_level_condition.h"
#include "game_server/server/extension/task/get_resource_condition.h"
#include "game_server/server/extension/task/get_vip_condition.h"
#include "game_server/server/extension/task/spend_resource_condition.h"
#include "game_server/server/extension/task/watering_condition.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace task {

bool ConditionPool::Initialize(size_t initial_number, size_t extend_number) {
  if(this->kill_mobs_.Initialize(initial_number, extend_number) == false) {
    global::LogError("%s:%d (%s) 初始化 KillMobCondition 对象池失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->talk_npcs_.Initialize(initial_number, extend_number) == false) {
    global::LogError("%s:%d (%s) 初始化 TalkNpcCondition 对象池失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->soul_appears_.Initialize(initial_number, extend_number) == false) {
    global::LogError("%s:%d (%s) 初始化 SoulAppearCondition 对象池失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->collect_items_.Initialize(initial_number, extend_number) == false) {
    global::LogError("%s:%d (%s) 初始化 CollectItemCondition 对象池失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->refine_equips_.Initialize(initial_number, extend_number) == false) {
    global::LogError("%s:%d (%s) 初始化 RefineEquipCondition 对象池失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->inlay_equips_.Initialize(initial_number, extend_number) == false) {
    global::LogError("%s:%d (%s) 初始化 InlayEquipCondition 对象池失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->strength_equips_.Initialize(initial_number, extend_number) == false) {
    global::LogError("%s:%d (%s) 初始化 StrengthEquipCondition 对象池失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->spirit_feeds_.Initialize(initial_number, extend_number) == false) {
    global::LogError("%s:%d (%s) 初始化 SpiritFeedCondition 对象池失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->use_items_.Initialize(initial_number, extend_number) == false) {
    global::LogError("%s:%d (%s) 初始化 UseItemCondition 对象池失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->divine_gems_.Initialize(initial_number, extend_number) == false) {
    global::LogError("%s:%d (%s) 初始化 DivineGemCondition 对象池失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->level_ups_.Initialize(initial_number, extend_number) == false) {
    global::LogError("%s:%d (%s) 初始化 LevelUpCondition 对象池失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->kill_mob_collects_.Initialize(initial_number, extend_number) == false) {
    global::LogError("%s:%d (%s) 初始化 KillMobCollectCondition 对象池失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->wear_equips_.Initialize(initial_number, extend_number) == false) {
    global::LogError("%s:%d (%s) 初始化 WearEquipCondition 对象池失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->soul_equips_.Initialize(initial_number, extend_number) == false) {
    global::LogError("%s:%d (%s) 初始化 SoulEquipCondition 对象池失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->soul_feeds_.Initialize(initial_number, extend_number) == false) {
    global::LogError("%s:%d (%s) 初始化 SoulFeedCondition 对象池失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->pass_playings_.Initialize(initial_number, extend_number) == false) {
    global::LogError("%s:%d (%s) 初始化 PassPlayingCondition 对象池失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->gather_.Initialize(initial_number, extend_number) == false) {
    global::LogError("%s:%d (%s) 初始化 GatherCondition 对象池失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->constellation_ups_.Initialize(initial_number, extend_number) == false) {
    global::LogError("%s:%d (%s) 初始化 ConstellationUpCondition 对象池失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->summon_npcs_.Initialize(initial_number, extend_number) == false) {
    LOG_ERROR("初始化 SummonNpcCondition 失败");
    return false;
  }
  if(this->kill_actors_.Initialize(initial_number, extend_number) == false) {
    LOG_ERROR("初始化 KillActorCondition 失败");
    return false;
  }
  if(this->kill_npc_types_.Initialize(initial_number, extend_number) == false) {
    LOG_ERROR("初始化 KillMobTypeCondition 失败");
    return false;
  }
  if(this->finish_task_types_.Initialize(initial_number, extend_number) == false) {
    LOG_ERROR("初始化 FinishTaskTypeCondition 失败");
    return false;
  }
  if(this->robs_.Initialize(initial_number, extend_number) == false) {
    LOG_ERROR("初始化 RobCondition 失败");
    return false;
  }
  if(this->kill_npc_levels_.Initialize(initial_number, extend_number) == false) {
    LOG_ERROR("初始化 KillMobLevelCondition 失败");
    return false;
  }
  if(this->spend_resources_.Initialize(initial_number, extend_number) == false) {
    LOG_ERROR("初始化 SpendResourceCondition 失败");
    return false;
  }
  if(this->get_resources_.Initialize(initial_number, extend_number) == false) {
    LOG_ERROR("初始化 GetResourceCondition 失败");
    return false;
  }
  if(this->waterings_.Initialize(initial_number, extend_number) == false) {
    LOG_ERROR("初始化 WateringCondition 失败");
    return false;
  }
  if(this->get_vips_.Initialize(initial_number, extend_number) == false) {
    LOG_ERROR("初始化 GetVipCondition 失败");
    return false;
  }

  return true;
}

void ConditionPool::Finalize() {
  this->kill_mobs_.Finalize();
  this->talk_npcs_.Finalize();
  this->soul_appears_.Finalize();
  this->collect_items_.Finalize();
  this->divine_gems_.Finalize();
  this->inlay_equips_.Finalize();
  this->kill_mob_collects_.Finalize();
  this->level_ups_.Finalize();
  this->refine_equips_.Finalize();
  this->strength_equips_.Finalize();
  this->spirit_feeds_.Finalize();
  this->use_items_.Finalize();
  this->wear_equips_.Finalize();
  this->soul_equips_.Finalize();
  this->soul_feeds_.Finalize();
  this->pass_playings_.Finalize();
  this->gather_.Finalize();
  this->constellation_ups_.Finalize();
  this->summon_npcs_.Finalize();
  this->kill_actors_.Finalize();
  this->kill_npc_types_.Finalize();
  this->finish_task_types_.Finalize();
  this->robs_.Finalize();
  this->kill_npc_levels_.Finalize();
  this->spend_resources_.Finalize();
  this->get_resources_.Finalize();
  this->waterings_.Finalize();
  this->get_vips_.Finalize();
}

Condition *ConditionPool::Allocate(entity::TaskCondition::type type) {
  Condition *condition = NULL;
  switch(type) {
    case entity::TaskCondition::KILL_MOB:
      condition = this->kill_mobs_.Allocate();
      break;
    case entity::TaskCondition::TALK:
      condition = this->talk_npcs_.Allocate();
      break;
    case entity::TaskCondition::PICK:
      condition = this->gather_.Allocate();
      break;
    case entity::TaskCondition::SOUL_APPEAR:
      condition = this->soul_appears_.Allocate();
      break;
    case entity::TaskCondition::USE_ITEM:
      condition = this->use_items_.Allocate();
      break;
    case entity::TaskCondition::COLLECT_ITEM:
      condition = this->collect_items_.Allocate();
      break;
    case entity::TaskCondition::WAITE_TIME:
      break;
    case entity::TaskCondition::EQUIP_STRENGHT:
      condition = this->strength_equips_.Allocate();
      break;
    case entity::TaskCondition::EQUIP_REFINE:
      condition = this->refine_equips_.Allocate();
      break;
    case entity::TaskCondition::DIVINE_GEM:
      condition = this->divine_gems_.Allocate();
      break;
    case entity::TaskCondition::SPIRIT_FEED:
      condition = this->spirit_feeds_.Allocate();
      break;
    case entity::TaskCondition::EQUIP_INLAY:
      condition = this->inlay_equips_.Allocate();
      break;
    case entity::TaskCondition::kill_COLLECT:
      condition = this->kill_mob_collects_.Allocate();
      break;
    case entity::TaskCondition::SOUL_FEED:
      condition = this->soul_feeds_.Allocate();
      break;
    case entity::TaskCondition::EQUIP_SOUL:
      condition = this->soul_equips_.Allocate();
      break;
    case entity::TaskCondition::ROLE_LV:
      condition = this->level_ups_.Allocate();
      break;
    case entity::TaskCondition::PASS_PLAYING:
      condition = this->pass_playings_.Allocate();
      break;
    case entity::TaskCondition::PROTECT_TASK:
      break;
    case entity::TaskCondition::ANSWER_TASK:
      break;
    case entity::TaskCondition::SPORTS_TASK_1:
      break;
    case entity::TaskCondition::SPORTS_TASK_2:
      break; 
    case entity::TaskCondition::EQUIPMENT:
      condition = this->wear_equips_.Allocate();
      break;
    case entity::TaskCondition::CONSTELLATION_UP:
      condition = this->constellation_ups_.Allocate();
      break;
    case entity::TaskCondition::SUMMON_NPC:
      condition = this->summon_npcs_.Allocate();
      break;
    case entity::TaskCondition::GET_RESOURCE:
      condition = this->get_resources_.Allocate();
      break;
    case entity::TaskCondition::SPEND_RESOURCE:
      condition = this->spend_resources_.Allocate();
      break;
    case entity::TaskCondition::ROB:
      condition = this->robs_.Allocate();
      break;
    case entity::TaskCondition::WATERING:
      condition = this->waterings_.Allocate();
      break;
    case entity::TaskCondition::KILL_ACTOR:
      condition = this->kill_actors_.Allocate();
      break;
    case entity::TaskCondition::KILL_MOB_LEVEL:
      condition = this->kill_npc_levels_.Allocate();
      break;
    case entity::TaskCondition::KILL_MOB_TYPE:
      condition = this->kill_npc_types_.Allocate();
      break;
    case entity::TaskCondition::FINISH_TASK_TYPE:
      condition = this->finish_task_types_.Allocate();
      break;
    case entity::TaskCondition::GET_VIP_PRIVILEGE:
      condition = this->get_vips_.Allocate();
      break;
    default:
      break;
  }
  return condition;
}

void ConditionPool::Deallocate(Condition *condition) {
  if(condition == NULL) {
    return ;
  }
  switch(condition->GetType()) {
    case entity::TaskCondition::KILL_MOB:
      this->kill_mobs_.Deallocate((KillMobCondition *)condition);
      break;
    case entity::TaskCondition::TALK:
      this->talk_npcs_.Deallocate((TalkNpcCondition *)condition);
      break;
    case entity::TaskCondition::PICK:
      this->gather_.Deallocate((GatherCondition *)condition);
      break;
    case entity::TaskCondition::SOUL_APPEAR:
      this->soul_appears_.Deallocate((SoulAppearCondition *)condition);
      break;
    case entity::TaskCondition::USE_ITEM:
      this->use_items_.Deallocate((UseItemCondition *)condition);
      break;
    case entity::TaskCondition::COLLECT_ITEM:
      this->collect_items_.Deallocate((CollectItemCondition *)condition);
      break;
    case entity::TaskCondition::WAITE_TIME:
      break;
    case entity::TaskCondition::EQUIP_STRENGHT:
      this->strength_equips_.Deallocate((StrengthEquipCondition *)condition);
      break;
    case entity::TaskCondition::EQUIP_REFINE:
      this->refine_equips_.Deallocate((RefineEquipCondition *)condition);
      break;
    case entity::TaskCondition::DIVINE_GEM:
      this->divine_gems_.Deallocate((DivineGemCondition *)condition);
      break;
    case entity::TaskCondition::SPIRIT_FEED:
      this->spirit_feeds_.Deallocate((SpiritFeedCondition *)condition);
      break;
    case entity::TaskCondition::EQUIP_INLAY:
      this->inlay_equips_.Deallocate((InlayEquipCondition *)condition);
      break;
    case entity::TaskCondition::SOUL_FEED:
      this->soul_equips_.Deallocate((SoulEquipCondition *)condition);
      break;
    case entity::TaskCondition::EQUIP_SOUL:
      this->soul_feeds_.Deallocate((SoulFeedCondition *)condition);
      break;
    case entity::TaskCondition::ROLE_LV:
      this->level_ups_.Deallocate((LevelUpCondition *)condition);
      break;
    case entity::TaskCondition::PASS_PLAYING:
      this->pass_playings_.Deallocate((PassPlayingCondition *)condition);
      break;
    case entity::TaskCondition::PROTECT_TASK:
      break;
    case entity::TaskCondition::ANSWER_TASK:
      break;
    case entity::TaskCondition::SPORTS_TASK_1:
      break;
    case entity::TaskCondition::SPORTS_TASK_2:
      break; 
    case entity::TaskCondition::EQUIPMENT:
      this->wear_equips_.Deallocate((WearEquipCondition *)condition);
      break;
    case entity::TaskCondition::CONSTELLATION_UP:
      this->constellation_ups_.Deallocate((ConstellationUpCondition *)condition);
      break;
    case entity::TaskCondition::SUMMON_NPC:
      this->summon_npcs_.Deallocate((SummonNpcCondition *)condition);
      break;
    case entity::TaskCondition::GET_RESOURCE:
      this->get_resources_.Deallocate((GetResourceCondition *)condition);
      break;
    case entity::TaskCondition::SPEND_RESOURCE:
      this->spend_resources_.Deallocate((SpendResourceCondition *)condition);
      break;
    case entity::TaskCondition::ROB:
      this->robs_.Deallocate((RobCondition *)condition);
      break;
    case entity::TaskCondition::WATERING:
      this->waterings_.Deallocate((WateringCondition *)condition);
      break;
    case entity::TaskCondition::KILL_ACTOR:
      this->kill_actors_.Deallocate((KillActorCondition *)condition);
      break;
    case entity::TaskCondition::KILL_MOB_LEVEL:
      this->kill_npc_levels_.Deallocate((KillMobLevelCondition *)condition);
      break;
    case entity::TaskCondition::KILL_MOB_TYPE:
      this->kill_npc_types_.Deallocate((KillMobTypeCondition *)condition);
      break;
    case entity::TaskCondition::FINISH_TASK_TYPE:
      this->finish_task_types_.Deallocate((FinishTaskTypeCondition *)condition);
      break;
    case entity::TaskCondition::GET_VIP_PRIVILEGE:
      this->get_vips_.Deallocate((GetVipCondition *)condition);
      break;
    default:
      break;
  }
}

}  // namespace task

}  // namespace server

}  // namespace game

