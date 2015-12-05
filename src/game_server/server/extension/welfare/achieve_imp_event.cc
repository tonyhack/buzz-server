
#include "game_server/server/extension/welfare/achieve_imp_event.h"

#include "entity/achieve_types.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/event/game_scene_event_types.h"
#include "game_server/server/event/game_attr_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension/welfare/condition.h"
#include "game_server/server/extension/welfare/task.h"
#include "game_server/server/event/game_skill_event_types.h"
#include "game_server/server/event/game_role_event_types.h"
#include "game_server/server/extension/welfare/achieve_actor.h"
#include "entity/gender_types.h"
#include "entity/actor_types.h"
#include "game_server/server/event/game_item_event_types.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "entity/spirit_types.h"
#include "game_server/server/extension/welfare/facede_request.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace welfare {

AchieveImpEvent::AchieveImpEvent() {}

AchieveImpEvent::~AchieveImpEvent() {}

bool AchieveImpEvent::Initialize(Condition *condition) {
  if(condition == NULL) {
    LOG_ERROR("条件初始化失败");
    return false;
  }
  this->conditon_ = condition;
  return true;
}

void AchieveImpEvent::Finalize() {

}

bool AchieveImpEvent::ExportVar(core::uint32 &param1, core::uint32 &param2, core::uint32 &value) {

return false;

}

bool AchieveImpEvent::CheckFinish() {
  return false;
}

void AchieveImpEvent::OnEvent(event::EventType::type type, const void *message, size_t size) {
  if(this->conditon_ == NULL) {
    LOG_ERROR("内存错误");
    return ;
  }
  entity::AchieveConditionType::type con_type = this->conditon_->GetType();
  switch(con_type) {
    case entity::AchieveConditionType::MIN:
      break;
    case entity::AchieveConditionType::SOUL_UNLOCK:
      break;
    case entity::AchieveConditionType::SOUL_UPGRAGE_LEVEL:
      break;
    case entity::AchieveConditionType::SKILL_USE:
      this->OnEventSkillUse(type, message, size);
      break;
    case entity::AchieveConditionType::FUNCTIONALITY_OPEN:
      this->OnEventFunctionalityOpen(type, message, size);
      break;
    case entity::AchieveConditionType::SPIRIT_UPGRADE_LEVEL:
      this->OnEventSpiritUpgradeLevel(type, message, size);
      break;
    case entity::AchieveConditionType::SPIRIT_COLLECT_SHAPE:
      break;
    case entity::AchieveConditionType::KILL_ROLE:
      this->OnEventKillRole(type, message, size);
      break;
    case entity::AchieveConditionType::BAG_EXPAND:
      break;
    case entity::AchieveConditionType::KILLED_SELF:
      this->OnEventKilledSelf(type, message, size);
      break;
    case entity::AchieveConditionType::EQUIP_STRENGTH:
      this->OnEventEquipStrength(type, message, size);
      break;
    case entity::AchieveConditionType::WEAR_EQUIP:
      this->OnEventWearEquip(type, message, size);
      break;
    case entity::AchieveConditionType::GEM_OPT:
      break;
    case entity::AchieveConditionType::GEM_STRENGTH:
      break;
    case entity::AchieveConditionType::RELIVE_WAY:
      this->OnEventReliveWay(type, message, size);
      break;
    case entity::AchieveConditionType::VOCATION_SET:
      break;
    case entity::AchieveConditionType::SOUL_SCHEME:
      break;
    case entity::AchieveConditionType::SWORD_UPGRADE:
      break;
    case entity::AchieveConditionType::SWORD_STRENGTH:
      break;
    case entity::AchieveConditionType::HOMELAND_BUILDING_UPGRADE:
      break;
    case entity::AchieveConditionType::HOMELAND_REVENUE:
      break;
    case entity::AchieveConditionType::HOMELAND_USE:
      break;
    case entity::AchieveConditionType::HOMELAND_WATER:
      break;
    case entity::AchieveConditionType::HOMELAND_OPT:
      break;
    case entity::AchieveConditionType::HOMELAND_ATTACK_NUM:
      break;
    case entity::AchieveConditionType::CORPS_OPT:
      break;
    case entity::AchieveConditionType::CORPS_JOB:
      break;
    case entity::AchieveConditionType::CORPS_GET_HOLY_CITY:
      break;
    case entity::AchieveConditionType::TO_SENCE:
      this->OnEventCorpsToScene(type, message, size);
      break;
    case entity::AchieveConditionType::NOLIBITY_GET_LEVEL:
      this->OnEventNolibityGetLevel(type, message, size);
      break;
    case entity::AchieveConditionType::USE_ITEM:
      this->OnEventUseItem(type, message, size);
      break;
    case entity::AchieveConditionType::RESOURCE:
      this->OnEventResource(type, message, size);
      break;
    case entity::AchieveConditionType::ACTOR_GET_VALUE:
      break;
    case entity::AchieveConditionType::PASS_STORY_COPY:
      break;
    case entity::AchieveConditionType::SELF_KILLED_SKILL:
      this->OnEventSelfKilledSkill(type, message, size);
      break;
    case entity::AchieveConditionType::KILLED_SELF_ACTOR_FIGHTING:
      this->OnEventKilledSelfFighting(type, message, size);
      break;
    case entity::AchieveConditionType::KILLED_SELF_ACTOR_LEVEL:
      this->OnEventKilledSelfLevel(type, message, size);
      break;
    case entity::AchieveConditionType::KILLED_SELF_ACTOR_APPEAR:
      this->OnEventKilledSelfAppear(type, message, size);
      break;
    case entity::AchieveConditionType::KILL_ACTOR_FIGHTING:
      this->OnEventKillActorFighting(type, message, size);
      break;
    case entity::AchieveConditionType::KILL_ACTOR_LEVEL:
      this->OnEventKillActorLevel(type, message, size);
      break;
    case entity::AchieveConditionType::KILL_ACTOR_APPEAR:
      this->OnEventKillActorAppear(type, message, size);
      break;
    case entity::AchieveConditionType::KILL_ACTOR_SEX:
      this->OnEventKillActorSex(type, message, size);
      break;
    case entity::AchieveConditionType::VIP_STATUS:
      this->OnEventVipStatus(type, message, size);
      break;
    case entity::AchieveConditionType::MAX:
      break;
  }
}

void AchieveImpEvent::Synchronize() {
  if(this->conditon_ == NULL) {
    LOG_ERROR("条件对象为空，无法同步数据");
    return ;
  }
  Task *task = this->conditon_->GetTask();
  if(task == NULL) {
    LOG_ERROR("任务对象为空");
    return ;
  }
  task->Synchronize();
}

// 事件处理
void AchieveImpEvent::OnEventSoulUnlock(event::EventType::type type, const void *data, size_t size) {}
void AchieveImpEvent::OnEventSoulUpgradeLevel(event::EventType::type type, const void *data, size_t size) {
}

void AchieveImpEvent::OnEventSkillUse(event::EventType::type type, const void *data, size_t size) {
  if(data == NULL || size == 0 || size != sizeof(event::EventUseSkill)) {
    LOG_ERROR("参数错误");
    return ;
  }

  if(type != event::EventType::EVENT_SKILL_USE_SKILL) {
    return ;
  }
  event::EventUseSkill *event = (event::EventUseSkill *)data;

  if(this->conditon_->param1_ == (core::uint32)entity::AchieveSkillUseType::ID) {
    if(this->conditon_->param2_ != (core::uint32)event->skill_id_) {
      return ;
    }
  } else if(this->conditon_->param1_ == (core::uint32)entity::AchieveSkillUseType::TYPE) {
    if(this->conditon_->param2_ != (core::uint32)event->type_) {
      return ;
    }
  }
  this->conditon_->value_ += 1;
  this->Synchronize();
}

void AchieveImpEvent::OnEventFunctionalityOpen(event::EventType::type type, const void *data, size_t size) {
  if(data == NULL || size == 0 || size != sizeof(event::EventActorFunctionalityEnable)) {
    LOG_ERROR("参数错误");
    return ;
  }

  event::EventActorFunctionalityEnable *enble = (event::EventActorFunctionalityEnable *)data;
  if((core::uint32)enble->functionality_id_ != this->conditon_->param1_) {
    return ;
  }
  this->conditon_->value_ += 1;
  this->Synchronize();
}

void AchieveImpEvent::OnEventSpiritUpgradeLevel(event::EventType::type type, const void *data, size_t size) {
  if(data == NULL || size == 0 || sizeof(event::EventAttrSpiritUpgrade) != size) {
    LOG_ERROR("参数错误");
    return ;
  }

  event::EventAttrSpiritUpgrade *event = (event::EventAttrSpiritUpgrade *)data;

  if(this->conditon_->param1_ > (core::uint32)event->new_level_) {
    return ;
  }

  this->conditon_->value_ = event->new_level_;
  this->Synchronize();
}

void AchieveImpEvent::OnEventSpiritCollectShape(event::EventType::type type, const void *data, size_t size) {}
void AchieveImpEvent::OnEventKillRole(event::EventType::type type, const void *data, size_t size) {
  if(type != event::EventType::EVENT_ROLE_KILLED) {
    LOG_ERROR("事件错误");
    return ;
  }
  if(data == NULL || size == 0 || sizeof(event::EventRoleKilled) != size) {
    LOG_ERROR("参数错误");
    return ;
  }
  
  event::EventRoleKilled *event = (event::EventRoleKilled *)data;

  if(event->attacker_type_ != entity::EntityType::TYPE_ACTOR) {
    return ;
  }
  GameActor *game_actor = this->GetActor();
  if(game_actor == NULL) {
    return ;
  }
  if(game_actor->GetActorID() != (core::uint64)event->attacker_id_) {
    return ;
  }
  if(this->conditon_->param1_ == (core::uint32)entity::AchieveKillRoleType::ANY_WAY) {

  } else if(this->conditon_->param1_ == (core::uint32)entity::AchieveKillRoleType::APPEAR_STATUS) {
    if(game_actor->CheckCommonStatus(entity::RoleCommonStatus::SOUL) == false) {
      return ;
    }
  } else if(this->conditon_->param1_ == (core::uint32)entity::AchieveKillRoleType::BEAST_STATUS) {
    if(FacedeRequest::GetInstance()->GetSpiritFacadeType((core::uint64)event->attacker_id_) != entity::SpiritFacadeType::HORSE) {
      return ;
    }
  } else if(this->conditon_->param1_ == (core::uint32)entity::AchieveKillRoleType::WING_STATUS) {
    if(FacedeRequest::GetInstance()->GetSpiritFacadeType((core::uint64)event->attacker_id_) != entity::SpiritFacadeType::WING) {
      return ;
    }
  } else {
    LOG_ERROR("参数错误");
  }

  // 计算参数2
  if(this->conditon_->param2_ == 0 && entity::EntityType::TYPE_ACTOR == event->type_) {
    this->conditon_->value_ += 1;
  }
  if(this->conditon_->param2_ != 0 && this->conditon_->param2_ == 
      FacedeRequest::GetInstance()->GetNpcTemplateID((core::uint64)event->id_)) {
    this->conditon_->value_ += 1;
  }
  this->Synchronize();
}

void AchieveImpEvent::OnEventBagExpand(event::EventType::type type, const void *data, size_t size) {}
void AchieveImpEvent::OnEventKilledSelf(event::EventType::type type, const void *data, size_t size) {
  if(type != event::EventType::EVENT_ROLE_KILLED) {
    LOG_ERROR("事件错误");
    return ;
  }
  if(data == NULL || size == 0 || sizeof(event::EventRoleKilled) != size) {
    LOG_ERROR("参数错误");
    return ;
  }

  event::EventRoleKilled *event = (event::EventRoleKilled *)data;

  if(event->type_ != entity::EntityType::TYPE_ACTOR) {
    return ;
  }

  GameActor *killed = this->GetActor();
  if(killed == NULL) {
    return ;
  }
  if(killed->GetActorID() != (core::uint64)event->id_) {
    return ;
  }
  if(entity::AchieveKilledSelfType::ACTOR == this->conditon_->param1_) {
    if(event->attacker_type_ != entity::EntityType::TYPE_ACTOR) {
      return ;
    }
  } else if(entity::AchieveKilledSelfType::ANY_WAY == this->conditon_->param1_) {

  } else if(entity::AchieveKilledSelfType::ANY_NPC == this->conditon_->param1_) {
    if(event->attacker_type_ != entity::EntityType::TYPE_NPC) {
      return ;
    }
  } else if(entity::AchieveKilledSelfType::SPECIALLY_NPC == this->conditon_->param1_) {
    if(event->attacker_type_ != entity::EntityType::TYPE_NPC) {
      return ;
    }
    if(this->conditon_->param2_ != FacedeRequest::GetInstance()->GetNpcTemplateID(event->attacker_id_)) {
      return ;
    }
  }
  this->conditon_->value_ += 1;
  this->Synchronize();
}

void AchieveImpEvent::OnEventEquipStrength(event::EventType::type type, const void *data, size_t size) {
  if(data == NULL || size == 0 || size != sizeof(event::EventItemEquipStrength)) {
    LOG_ERROR("参数错误");
    return ;
  }
  event::EventItemEquipStrength *event = (event::EventItemEquipStrength *)data;
  if(this->conditon_->param1_ == entity::AchieveEquipStrengthType::SUCCESS) {
    if(event->result_ != true) {
      return ;
    }
  } else if(this->conditon_->param1_ == entity::AchieveEquipStrengthType::FAILED) {
    if(event->result_ != false) {
      return ;
    }
  }

  this->conditon_->value_ += 1;
  this->Synchronize();
}

void AchieveImpEvent::OnEventWearEquip(event::EventType::type type, const void *data, size_t size) {
  if(data == NULL || size == 0 || size != sizeof(event::EventItemWearEquip)) {
    LOG_ERROR("参数错误");
    return ;
  }
  event::EventItemWearEquip *event = (event::EventItemWearEquip *)data;
  if(FacedeRequest::GetInstance()->RequestActorEquipNum(event->id_, (entity::AchieveWearEquipType::type)this->conditon_->param1_,
        this->conditon_->param2_) < (core::int32)this->conditon_->param3_) {
    return ;
  }

  this->conditon_->value_ = this->conditon_->param3_;
  this->Synchronize();
}

void AchieveImpEvent::OnEventGemOpt(event::EventType::type type, const void *data, size_t size) {}
void AchieveImpEvent::OnEventGemInlay(event::EventType::type type, const void *data, size_t size) {}
void AchieveImpEvent::OnEventReliveWay(event::EventType::type type, const void *data, size_t size) {
  if(data == NULL || size == 0 || size != sizeof(event::EventActorRelive)) {
    LOG_ERROR("参数错误");
  }
  event::EventActorRelive *event = (event::EventActorRelive *)data;
  if((core::uint32)event->type_ == this->conditon_->param1_) {
    this->conditon_->value_ += 1;
    this->Synchronize();
  }
}

void AchieveImpEvent::OnEventVocationSet(event::EventType::type type, const void *data, size_t size) {}
void AchieveImpEvent::OnEventSoulScheme(event::EventType::type type, const void *data, size_t size) {}
void AchieveImpEvent::OnEventSwordUpgrade(event::EventType::type type, const void *data, size_t size) {}
void AchieveImpEvent::OnEventSwordInlay(event::EventType::type type, const void *data, size_t size) {}
void AchieveImpEvent::OnEventHomelandBuildingUpgrade(event::EventType::type type, const void *data, size_t size) {}
void AchieveImpEvent::OnEventHomelandRevenue(event::EventType::type type, const void *data, size_t size) {}
void AchieveImpEvent::OnEventHomelandUse(event::EventType::type type, const void *data, size_t size) {}
void AchieveImpEvent::OnEventHomelandWater(event::EventType::type type, const void *data, size_t size) {}
void AchieveImpEvent::OnEventHomelandOpt(event::EventType::type type, const void *data, size_t size) {}
void AchieveImpEvent::OnEventHomelandAttackNum(event::EventType::type type, const void *data, size_t size) {}
void AchieveImpEvent::OnEventCorpsOpt(event::EventType::type type, const void *data, size_t size) {}
void AchieveImpEvent::OnEventCorpsJob(event::EventType::type type, const void *data, size_t size) {}
void AchieveImpEvent::OnEventCorpsGetHolyCity(event::EventType::type type, const void *data, size_t size) {}
void AchieveImpEvent::OnEventCorpsToScene(event::EventType::type type, const void *data, size_t size) {
  if(data == NULL || size == 0 || sizeof(event::EventSceneJumpMap) != size) {
    LOG_ERROR("参数错误");
    return ;
  }
  this->conditon_->value_ += 1;
  this->Synchronize();
}

void AchieveImpEvent::OnEventNolibityGetLevel(event::EventType::type type, const void *data, size_t size) {}
void AchieveImpEvent::OnEventUseItem(event::EventType::type type, const void *data, size_t size) {
  if(data == NULL || size == 0 || size != sizeof(event::EventItemUse)) {
    LOG_ERROR("参数错误");
    return ;
  }
  event::EventItemUse *event = (event::EventItemUse *)data;
  if((core::uint32)event->template_id_ != this->conditon_->param1_) {
    return ;
  }

  this->conditon_->value_ += 1;
  this->Synchronize();
}

void AchieveImpEvent::OnEventResource(event::EventType::type type, const void *data, size_t size) {
  if(type != event::EventType::EVENT_ACTOR_RESOURCE_CHANGE) {
    return ;
  }
  if(data == NULL || size != sizeof(event::EventActorResourceChange)) {
    LOG_ERROR("参数 data 或 size 错误");
    return ;
  }
  event::EventActorResourceChange *event = (event::EventActorResourceChange *)data;
  if(this->conditon_->param1_ == entity::AchieveResourceType::GET) {
    if(event->old_num_ < event->new_num_ && (core::uint32)event->type_ == this->conditon_->param2_) {
      this->conditon_->value_ += event->new_num_- event->old_num_;
      this->Synchronize();
    }
  } else if(this->conditon_->param1_ == entity::AchieveResourceType::SPEND) {
    if(event->old_num_ > event->new_num_ && (core::uint32)event->type_ == this->conditon_->param2_) {
      this->conditon_->value_ += event->old_num_ - event->new_num_;
      this->Synchronize();
    }
  } else {
    LOG_ERROR("条件参数错误");
  }

  this->conditon_->value_ += 1;
  this->Synchronize();
}

void AchieveImpEvent::OnEventActorGetValue(event::EventType::type type, const void *data, size_t size) {}
void AchieveImpEvent::OnEventPassStoryCopy(event::EventType::type type, const void *data, size_t size) {}
void AchieveImpEvent::OnEventSelfKilledSkill(event::EventType::type type, const void *data, size_t size) {
  if(type != event::EventType::EVENT_ROLE_KILLED) {
    return ;
  }
  if(data == NULL || size == 0 || sizeof(event::EventRoleKilled) != size) {
    LOG_ERROR("参数错误");
    return ;
  }

  event::EventRoleKilled *event = (event::EventRoleKilled *)data;

  if(event->type_ != entity::EntityType::TYPE_ACTOR) {
    return ;
  }

  GameActor *killed = this->GetActor();
  if(killed == NULL) {
    LOG_ERROR("玩家没有找到");
    return ;
  }
  if(killed->GetActorID() != (core::uint64)event->id_) {
    return ;
  }

  if((core::uint32)event->skill_id_ != this->conditon_->param1_) {
    return ;
  }
  this->conditon_->value_ += 1;
  this->Synchronize();
}

void AchieveImpEvent::OnEventKillActorFighting(event::EventType::type type, const void *data, size_t size) {
  if(type != event::EventType::EVENT_ROLE_KILLED) {
    return ;
  }
  if(data == NULL || size == 0 || sizeof(event::EventRoleKilled) != size) {
    LOG_ERROR("参数错误");
    return ;
  }

  event::EventRoleKilled *event = (event::EventRoleKilled *)data;

  if(event->type_ != entity::EntityType::TYPE_ACTOR || event->attacker_type_ != entity::EntityType::TYPE_ACTOR) {
    return ;
  }

  GameActor *killer = this->GetActor();
  if(killer == NULL) {
    return ;
  }
  if(killer->GetActorID() != (core::uint64)event->attacker_id_) {
    return ;
  }

  GameActor *killed = GameActorManager::GetInstance()->GetActor(event->id_);
  if(killed == NULL) {
    LOG_ERROR("玩家对象没有找到");
    return ;
  }

  if(this->conditon_->param1_ == 1) {
    if(killed->GetCurrentFightScore() - killer->GetCurrentFightScore() >= (core::int32)this->conditon_->param2_) {
      this->conditon_->value_ += 1;
      this->Synchronize();
    }
  } else if(this->conditon_->param1_ == 0) {
    if(killer->GetCurrentFightScore() - killed->GetCurrentFightScore() >= (core::int32)this->conditon_->param2_) {
      this->conditon_->value_ += 1;
      this->Synchronize();
    }
  } else {
    LOG_ERROR("参数错误");
    return ;
  }
}

void AchieveImpEvent::OnEventKillActorLevel(event::EventType::type type, const void *data, size_t size) {
  if(type != event::EventType::EVENT_ROLE_KILLED) {
    return ;
  }
  if(data == NULL || size == 0 || sizeof(event::EventRoleKilled) != size) {
    LOG_ERROR("参数错误");
    return ;
  }

  event::EventRoleKilled *event = (event::EventRoleKilled *)data;

  if(event->type_ != entity::EntityType::TYPE_ACTOR || event->attacker_type_ != entity::EntityType::TYPE_ACTOR) {
    return ;
  }

  GameActor *killer = this->GetActor();
  if(killer == NULL) {
    LOG_ERROR("玩家对象没有找到");
    return ;
  }
  if(killer->GetActorID() != (core::uint64)event->attacker_id_) {
    return ;
  }

  GameActor *killed = GameActorManager::GetInstance()->GetActor(event->id_);
  if(killed == NULL) {
    LOG_ERROR("玩家对象没有找到");
    return ;
  }

  if(this->conditon_->param1_ == 1) {
    if(killed->GetAttribute(entity::RoleAoiFields::LEVEL) - killer->GetAttribute(entity::RoleAoiFields::LEVEL) >= (core::int32)this->conditon_->param2_) {
      this->conditon_->value_ += 1;
      this->Synchronize();
    }
  } else if(this->conditon_->param1_ == 0) {
    if(killer->GetAttribute(entity::RoleAoiFields::LEVEL) - killed->GetAttribute(entity::RoleAoiFields::LEVEL) >= (core::int32)this->conditon_->param2_) {
      this->conditon_->value_ += 1;
      this->Synchronize();
    } else {
      LOG_ERROR("参数错误");
      return ;
    }
  }
}

void AchieveImpEvent::OnEventKillActorAppear(event::EventType::type type, const void *data, size_t size) {
  if(type != event::EventType::EVENT_ROLE_KILLED) {
    return ;
  }
  if(data == NULL || size == 0 || sizeof(event::EventRoleKilled) != size) {
    LOG_ERROR("参数错误");
    return ;
  }

  event::EventRoleKilled *event = (event::EventRoleKilled *)data;

  if(event->type_ != entity::EntityType::TYPE_ACTOR || event->attacker_type_ != entity::EntityType::TYPE_ACTOR) {
    return ;
  }

  GameActor *killer = this->GetActor();
  if(killer == NULL) {
    LOG_ERROR("玩家对象没有找到");
    return ;
  }
  if(killer->GetActorID() != (core::uint64)event->attacker_id_) {
    return ;
  }

  GameActor *killed = GameActorManager::GetInstance()->GetActor(event->id_);
  if(killed == NULL) {
    LOG_ERROR("玩家对象没有找到");
    return ;
  }

  if(this->conditon_->param1_ == 0) {
    if(killer->CheckCommonStatus(entity::RoleCommonStatus::SOUL) == false) {
      return ;
    }
  } else {
    if(FacedeRequest::GetInstance()->GetBuffExist(killed->GetActorID(), this->conditon_->param2_) == false) {
      return ;
    }
  }

  this->conditon_->value_ += 1;
  this->Synchronize();
}

void AchieveImpEvent::OnEventKillActorSex(event::EventType::type type, const void *data, size_t size) {
  if(type != event::EventType::EVENT_ROLE_KILLED) {
    return ;
  }
  if(data == NULL || size == 0 || sizeof(event::EventRoleKilled) != size) {
    LOG_ERROR("参数错误");
    return ;
  }

  event::EventRoleKilled *event = (event::EventRoleKilled *)data;

  if(event->type_ != entity::EntityType::TYPE_ACTOR || event->attacker_type_ != entity::EntityType::TYPE_ACTOR) {
    return ;
  }

  GameActor *killer = this->GetActor();
  if(killer == NULL) {
    LOG_ERROR("玩家对象没有找到");
    return ;
  }
  if(killer->GetActorID() != (core::uint64)event->attacker_id_) {
    return ;
  }

  GameActor *killed = GameActorManager::GetInstance()->GetActor(event->id_);
  if(killed == NULL) {
    LOG_ERROR("玩家对象没有找到");
    return ;
  }

  if(this->conditon_->param1_ == 0) {
    if(killed->GetAttribute(entity::ActorAoiFields::GENDER) != entity::GenderType::MALE) {
      return ;
    }
  } else if(this->conditon_->param1_ == 1) {
    if(killed->GetAttribute(entity::ActorAoiFields::GENDER) != entity::GenderType::FAMAIL) {
      return ;
    }
  } else if(this->conditon_->param1_ == 2) {
    if(killed->GetAttribute(entity::ActorAoiFields::GENDER) != killer->GetAttribute(entity::ActorAoiFields::GENDER)) {
      return ;
    }
  } else if(this->conditon_->param1_ == 3) {
    if(killed->GetAttribute(entity::ActorAoiFields::GENDER) == killer->GetAttribute(entity::ActorAoiFields::GENDER)) {
      return ;
    } 
  } else {
    LOG_ERROR("参数错误");
    return ;
  }

  this->conditon_->value_ += 1;
  this->Synchronize();
}

void AchieveImpEvent::OnEventVipStatus(event::EventType::type type, const void *data, size_t size) {
  if(type != event::EventType::EVENT_ACTOR_VIP_CHANGE) {
    return ;
  }
  if(data == NULL || size == 0 || sizeof(event::EventActorVipChange) != size) {
    LOG_ERROR("参数错误");
    return ;
  }

  event::EventActorVipChange *event = (event::EventActorVipChange *)data;

  if((core::uint32)event->new_type_ >= this->conditon_->param1_) {
    this->conditon_->value_ += 1;
    this->Synchronize();
  }
}

GameActor *AchieveImpEvent::GetActor() {
  Task *task = this->conditon_->GetTask();
  if(task == NULL) {
    LOG_ERROR("任务对象没有找到");
    return NULL;
  }
  AchieveActor *actor = task->GetActor(); 
  if(actor == NULL) {
    LOG_ERROR("成就对象没有找到");
    return NULL;
  }

  return actor->GetActor();
}

void AchieveImpEvent::OnEventKilledSelfFighting(event::EventType::type type, const void *data, size_t size) {
  if(type != event::EventType::EVENT_ROLE_KILLED) {
    return ;
  }
  if(data == NULL || size == 0 || sizeof(event::EventRoleKilled) != size) {
    LOG_ERROR("参数错误");
    return ;
  }

  event::EventRoleKilled *event = (event::EventRoleKilled *)data;

  if(event->type_ != entity::EntityType::TYPE_ACTOR || event->attacker_type_ != entity::EntityType::TYPE_ACTOR) {
    return ;
  }

  GameActor *killed = this->GetActor();
  if(killed == NULL) {
    LOG_ERROR("玩家对象没有找到");
    return ;
  }
  if(killed->GetActorID() != (core::uint64)event->id_) {
    return ;
  }

  GameActor *killer = GameActorManager::GetInstance()->GetActor(event->attacker_id_);
  if(killer == NULL) {
    LOG_ERROR("被杀的玩家没有找到(%lu)", event->attacker_id_);
    return ;
  }

  if(this->conditon_->param1_ == entity::AchieveSelfKilledType::FIGHTING) {
    if(this->conditon_->param1_ == 1) {
      if(killer->GetCurrentFightScore() - killed->GetCurrentFightScore() >= (core::int32)this->conditon_->param2_) {
        this->conditon_->value_ += 1;
        this->Synchronize();
      }
    } else if(this->conditon_->param1_ == 2) {
      if(killed->GetCurrentFightScore() - killer->GetCurrentFightScore() >= (core::int32)this->conditon_->param2_) {
        this->conditon_->value_ += 1;
        this->Synchronize();
      }
    }
  }
}

void AchieveImpEvent::OnEventKilledSelfLevel(event::EventType::type type, const void *data, size_t size) {
  if(type != event::EventType::EVENT_ROLE_KILLED) {
    return ;
  }
  if(data == NULL || size == 0 || sizeof(event::EventRoleKilled) != size) {
    LOG_ERROR("参数错误");
    return ;
  }

  event::EventRoleKilled *event = (event::EventRoleKilled *)data;

  if(event->type_ != entity::EntityType::TYPE_ACTOR || event->attacker_type_ != entity::EntityType::TYPE_ACTOR) {
    return ;
  }

  GameActor *killed = this->GetActor();
  if(killed == NULL) {
    LOG_ERROR("玩家对象没有找到");
    return ;
  }
  if(killed->GetActorID() != (core::uint64)event->id_) {
    return ;
  }

  GameActor *killer = GameActorManager::GetInstance()->GetActor(event->attacker_id_);
  if(killer == NULL) {
    LOG_ERROR("被杀的玩家没有找到(%lu)", event->attacker_id_);
    return ;
  }

  if(this->conditon_->param1_ == 1) {
    if(killer->GetAttribute(entity::RoleAoiFields::LEVEL) - killed->GetAttribute(entity::RoleAoiFields::LEVEL)
        >= (core::int32)this->conditon_->param2_ ) {
      this->conditon_->value_ += 1;
      this->Synchronize();
    }
  } else if(this->conditon_->param1_ == 0) {
    if(killed->GetAttribute(entity::RoleAoiFields::LEVEL) - killer->GetAttribute(entity::RoleAoiFields::LEVEL)
        >= (core::int32)this->conditon_->param2_ ) {
      this->conditon_->value_ += 1;
      this->Synchronize();
    }
  }
}

void AchieveImpEvent::OnEventKilledSelfAppear(event::EventType::type type, const void *data, size_t size) {
  if(type != event::EventType::EVENT_ROLE_KILLED) {
    return ;
  }
  if(data == NULL || size == 0 || sizeof(event::EventRoleKilled) != size) {
    LOG_ERROR("参数错误");
    return ;
  }

  event::EventRoleKilled *event = (event::EventRoleKilled *)data;

  if(event->type_ != entity::EntityType::TYPE_ACTOR || event->attacker_type_ != entity::EntityType::TYPE_ACTOR) {
    return ;
  }

  GameActor *killed = this->GetActor();
  if(killed == NULL) {
    LOG_ERROR("玩家对象没有找到");
    return ;
  }
  if(killed->GetActorID() != (core::uint64)event->id_) {
    return ;
  }

  GameActor *killer = GameActorManager::GetInstance()->GetActor(event->attacker_id_);
  if(killer == NULL) {
    LOG_ERROR("被杀的玩家没有找到(%lu)", event->attacker_id_);
    return ;
  }

  if(this->conditon_->param1_ == 0) {
    if(killer->CheckCommonStatus(entity::RoleCommonStatus::SOUL) == false) {
      return ;
    } else {
      if(FacedeRequest::GetInstance()->GetBuffExist(killer->GetActorID(), this->conditon_->param2_) == false) {
        return ;
      }
    }
  }

  this->conditon_->value_ += 1;
  this->Synchronize();
}

}  // namespace welfare

}  // namespace server

}  // namespace game
