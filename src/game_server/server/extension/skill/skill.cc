//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-05 12:16:07.
// File name: skill.cc
//
// Description:
// Define class Skill.
//

#include "game_server/server/extension/skill/skill.h"

#include "coresh/communicator.h"
#include "entity/camp_types.h"
#include "entity/actor_types.h"
#include "game_server/server/according_level_configure.h"
#include "game_server/server/configure.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_npc.h"
#include "game_server/server/extension/skill/configure.h"
#include "game_server/server/extension/skill/damage_calculation.h"
#include "game_server/server/extension/skill/facade_request.h"
#include "game_server/server/extension/skill/skill_configure.h"
#include "game_server/server/extension/skill/skill_role.h"
#include "game_server/server/extension/skill/skill_role_manager.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_role_event_types.h"
#include "game_server/server/event/game_skill_event_types.h"
#include "game_server/server/request/game_ai_request_types.h"
#include "game_server/server/request/game_buff_request_types.h"
#include "game_server/server/event/game_skill_event_types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_skill_protocol_types.h"
#include "global/common_functions.h"
#include "global/direction_calculator.h"
#include "global/logging.h"
#include "global/types.h"

namespace game {

namespace server {

namespace skill {

Skill::Skill() : id_(0), level_(0) {}
Skill::~Skill() {}

bool Skill::Initialize(SkillRole *role) {
  if (role == NULL) {
    LOG_ERROR("参数 role 为空");
    return false;
  }

  this->attributes_.resize(entity::SkillVarFields::SKILL_END, 0);

  this->role_ = role;

  return true;
}

void Skill::Finalize() {
  this->id_ = 0;
  this->level_ = 0;
  this->role_ = NULL;
  this->attributes_.assign(entity::SkillVarFields::SKILL_END, 0);
}

SkillResultType::type Skill::CheckUseSkill(SkillRole *role) {
  if(role == NULL) {
    return SkillResultType::ERROR_ARGS;
  }

  GameRole *game_role = role->GetGameRole();
  if(game_role == NULL) {
    return SkillResultType::ERROR_ARGS;
  }

  // 角色是否处于禁止使用技能状态
  if(game_role->CheckCommonStatus(entity::RoleCommonStatus::FORBID_USE_SKILL)) {
    return SkillResultType::ERROR_FORBID_SKILL;
  }

  const SkillContext &context = role->GetSkillContext();

  // 获取技能配置
  const SkillCell *cell = Configure::GetInstance()->GetSkill(
      this->GetID(), this->GetLevel());
  if(cell == NULL) {
    return SkillResultType::ERROR_SKILL_ID;
  }

  // 技能是否在待用状态
  if(role->CheckSkillReady(context.id_) == false) {
    return SkillResultType::ERROR_SKILL_SIT;
  }

  if (role->CheckBuffCondition(cell) == false) {
    return SkillResultType::ERROR_BUFF_STATUS;
  }

  if(role->CheckSoulCondition(cell) == false) {
    return SkillResultType::ERROR_SOUL_STATUS;
  }

  SkillResultType::type result = SkillResultType::MAX;

  // 是否满足技能释放条件
  if((result = role->CheckTarget(cell)) != SkillResultType::SUCCESS ||
      (result = role->CheckDistance(cell)) != SkillResultType::SUCCESS ||
      (result = role->CheckCooling(cell)) != SkillResultType::SUCCESS ||
      (result = role->CheckConsume(cell)) != SkillResultType::SUCCESS ||
      (result = role->CheckCondition(cell)) != SkillResultType::SUCCESS) {
    return result;
  }

  return SkillResultType::SUCCESS;
}


SkillResultType::type Skill::CheckLaunchSkill(SkillRole *role) {
  if(role == NULL) {
    return SkillResultType::ERROR_ARGS;
  }

  const SkillContext &context = role->GetSkillContext();

  // 获取技能配置
  const SkillCell *cell = Configure::GetInstance()->GetSkill(
      this->GetID(), this->GetLevel());
  if(cell == NULL) {
    return SkillResultType::ERROR_SKILL_ID;
  }

  SkillResultType::type result = SkillResultType::MAX;

  // 是否满足技能释放条件
  if((result = role->CheckCooling(cell)) != SkillResultType::SUCCESS ||
      (result = role->CheckConsume(cell)) != SkillResultType::SUCCESS ||
      (result = role->CheckCondition(cell)) != SkillResultType::SUCCESS) {
    return result;
  }

  // 获取与目标的距离
  core::int32 distance = role->GetSkillDistance(cell);
  core::int32 distance_add =
    this->GetAttribute(entity::SkillVarFields::DISTANCE_ADD_VALUE);

  // 大于吟唱距离，发射失败
  if(distance == core::kint32max ||
      distance > cell->sing_distance_ + distance_add) {
    return SkillResultType::ERROR_DISTANCE;
  }

  return SkillResultType::SUCCESS;
}

SkillResultType::type Skill::OnLaunchSkill(SkillRole *role) {
  if(role == NULL) {
    return SkillResultType::ERROR_ARGS;
  }

  GameRole *game_role = role->GetGameRole();
  if(game_role == NULL) {
    return SkillResultType::ERROR_ARGS;
  }

  const SkillContext &context = role->GetSkillContext();

  // 获取技能配置
  const SkillCell *cell = Configure::GetInstance()->GetSkill(
      this->GetID(), this->GetLevel());
  if(cell == NULL) {
    return SkillResultType::ERROR_SKILL_ID;
  }

  // 检测技能是否满足释放条件
  SkillResultType::type result = this->CheckLaunchSkill(role);
  if(result != SkillResultType::SUCCESS) {
    return result;
  }

  // 广播技能发射到周围，包括自己
  gateway::protocol::MessageSkillOperate message;
  message.__set_skill_id_(this->GetID());
  message.__set_type_(gateway::protocol::SkillOperateType::LAUNCH);
  message.context_.__set_src_type_(game_role->GetType());
  message.context_.__set_src_id_(game_role->GetStringID());
  if(cell->target_type_ == TargetType::ENTITY) {
    message.context_.__set_target_type_(context.target_type_);
    message.context_.__set_target_id_(global::ToString(context.target_id_));
  } else if(cell->target_type_ == TargetType::POSITION ||
      cell->target_type_ == TargetType::POSITION_SELF) {
    message.context_.__set_x_(context.x_);
    message.context_.__set_y_(context.y_);
  } else if(cell->type_ == SkillType::COLLISION &&
      cell->target_type_ == TargetType::NONE) {
    message.context_.__set_direction_(context.direction_);
  } else {
    return SkillResultType::ERROR_TARGET;
  }
  game_role->SendMessageAround(message,
      gateway::protocol::MessageType::MESSAGE_SKILL_OPERATE, true);

  // 技能冷却
  role->SkillCooling(cell, this->GetAttribute(entity::SkillVarFields::COOLING_ADD_VALUE));
  // 技能消耗
  role->SkillConsume(cell);

  return SkillResultType::SUCCESS;
}

SkillResultType::type Skill::OnFinishSkill(SkillRole *role) {
  if(role == NULL) {
    return SkillResultType::ERROR_ARGS;
  }

  // global::LogDebug("[技能流程] 技能进行伤害计算");

  const SkillContext &context = role->GetSkillContext();

  // 获取技能配置
  const SkillCell *cell = Configure::GetInstance()->GetSkill(
      this->GetID(), this->GetLevel());
  if(cell == NULL) {
    return SkillResultType::ERROR_SKILL_ID;
  }

  if(cell->target_type_ == TargetType::ENTITY) {
    // 目标
    SkillRole *dest = SkillRoleManager::GetInstance()->Get(context.target_type_,
        context.target_id_);
    if(dest == NULL) {
      return SkillResultType::ERROR_ARGS;
    }
    GameRole *game_role_dest = dest->GetGameRole();
    if(game_role_dest == NULL) {
      return SkillResultType::ERROR_TARGET;
    }
    // 目标死亡直接返回
    if(game_role_dest->GetBasicStatus() == entity::RoleBasicStatus::STATUS_DIE) {
      return SkillResultType::SUCCESS;
    }
  } else if(cell->target_type_ == TargetType::POSITION ||
      cell->target_type_ == TargetType::POSITION_SELF) {
  } else if(cell->type_ == SkillType::COLLISION &&
      cell->target_type_ == TargetType::NONE) {
  } else {
    return SkillResultType::ERROR_TARGET;
  }

  if (role->GetType() == entity::EntityType::TYPE_ACTOR && cell->extra_aoe_hurt_id_ > 0) {
    entity::SoulEnergyStatus::type energy_status =
      FacadeRequest::GetInstance()->GetEnergyStatus(role->GetID());
    bool extra_aoe = false;
    switch (energy_status) {
      case entity::SoulEnergyStatus::EXPLOSION:
        extra_aoe = cell->extra_aoe_condition_ ==
          ExtraAoeConditionType::SOUL_ENERGY_EXPLOSION;
        break;
      default:
        break;
    }
    if (extra_aoe == true) {
      this->SkillHitExtraAoe(role);
    }
  }

  SkillResultType::type result = SkillResultType::ERROR_UNKNOWN;

  if(cell->type_ == SkillType::MOMENTARY || cell->type_ == SkillType::REMOTE) {
    if(cell->range_type_ == RangeType::SINGLE) {
      result = this->SkillHitSingle(role);
    } else if(cell->range_type_ == RangeType::CIRCLE) {
      result = this->SkillHitCircle(role);
    } else if(cell->range_type_ == RangeType::SECTOR_QUARTER) {
      result = this->SkillHitSector(role, request::SectorRadianType::QUARTER);
    } else if(cell->range_type_ == RangeType::SECTOR_HALF) {
      result = this->SkillHitSector(role, request::SectorRadianType::HALF);
    } else {
      return SkillResultType::ERROR_ARGS;
    }
    if (result != SkillResultType::SUCCESS) {
      return result;
    }
  }

  return SkillResultType::SUCCESS;
}

SkillResultType::type Skill::SkillHitSingle(SkillRole *role) {
  if(role == NULL) {
    return SkillResultType::ERROR_ARGS;
  }

  const SkillContext &context = role->GetSkillContext();

  // 获取技能配置
  const SkillCell *cell = Configure::GetInstance()->GetSkill(
      this->GetID(), this->GetLevel());
  if(cell == NULL) {
    return SkillResultType::ERROR_SKILL_ID;
  }

  // 是否为单体范围(目标必然是实体)
  if(cell->range_type_ != RangeType::SINGLE ||
      cell->target_type_ != TargetType::ENTITY) {
    return SkillResultType::ERROR_ARGS;
  }

  // 目标
  SkillRole *dest = SkillRoleManager::GetInstance()->Get(context.target_type_,
      context.target_id_);
  if(dest == NULL) {
    return SkillResultType::ERROR_ARGS;
  }

  if(cell->hurt_id_ > 0) {
    // 伤害计算
    // return this->HurtCalculate(role, dest, cell->hurt_id_);
    SkillResultType::type ret = DamageCalculation::GetInstance()->HurtCalculate(role, dest,
        cell->hurt_id_, cell->hit_type_, this);
    if (ret != SkillResultType::SUCCESS) {
      return ret;
    }
  }

  // 随机数
  core::uint32 rand =
    ExtensionManager::GetInstance()->GetRandom().Random(global::kPercentDenom);

  // 加BUFF请求
  request::RequestBuffStart request;
  request.__set_dest_entity_type_(context.target_type_);
  request.__set_dest_entity_id_(context.target_id_);
  request.__set_src_entity_type_(role->GetType());
  request.__set_src_entity_id_(role->GetID());

  // 技能BUFF
  std::map<core::uint32 ,core::uint32>::const_iterator iterator = cell->buffs_.begin();
  for(; iterator != cell->buffs_.end(); ++iterator) {
    if(iterator->second >= rand) {
      request.__set_buff_id_(iterator->first);
      // 请求发送
      ExtensionManager::GetInstance()->Request(
          request::RequestType::REQUEST_BUFF_START, &request, sizeof(request));
    }
  }

  // 公共 Debuffs 只有在伤害类型技能中才会触发
  if(cell->friend_type_ == FriendType::HURT) {
    SkillRole::DebuffVector &debuffs = role->GetGlobalDebuffs();
    if(debuffs.empty() == false) {
      core::uint32 rand =
        ExtensionManager::GetInstance()->GetRandom().Random(global::kPercentDenom);
      size_t pos = rand % debuffs.size();
      if(rand <= debuffs[pos].odds_ &&
          FacadeRequest::GetInstance()->CheckCooling(role->GetType(), role->GetID(),
            debuffs[pos].cooling_) == false) {
        // 冷却
        FacadeRequest::GetInstance()->Cooling(role->GetType(), role->GetID(),
            debuffs[pos].cooling_);
        this->AddGlobalBuff(role, dest, debuffs[pos].buff_, (entity::BuffTargetType::type)
            debuffs[pos].target_type_);
      }
    }
  }

  // 这里如果技能需要在非幻化条件下才可以用，说明是幻化类技能
  if(cell->classify_type_ == ClassifyType::SOUL) {
    event::ChannelType::type channel_type = event::ChannelType::CHANNEL_MAX;
    switch(role->GetType()) {
      case entity::EntityType::TYPE_ACTOR:
        channel_type = event::ChannelType::CHANNEL_ACTOR;
        break;
      case entity::EntityType::TYPE_NPC:
        channel_type = event::ChannelType::CHANNEL_NPC;
        break;
      default:
        break;
    }
    // 广播使用幻化技能的事件
    event::EventUseAppearSkill event;
    event.__set_type_(role->GetType());
    event.__set_id_(role->GetID());
    coresh::CommunicatorSingleton::GetInstance().Broadcast(
        event::EventType::EVENT_SKILL_USE_APPEAR, role->GetID(),
        channel_type, &event, sizeof(event::EventUseAppearSkill));
  }
  
  // 广播受到攻击事件
  this->OnRoleAttacked(dest, role, cell);
  
  return SkillResultType::SUCCESS;
}

void Skill::AddGlobalBuff(SkillRole *role, SkillRole *target_role, core::int32 buff,
    entity::BuffTargetType::type target_type) {
  if(role == NULL || target_role == NULL) {
    LOG_ERROR("角色没有找到");
    return;
  }
  if(target_type == entity::BuffTargetType::SELF) {
    FacadeRequest::GetInstance()->AddBuff(role->GetType(), role->GetID(),
        role->GetType(), role->GetID(), buff);
  }
  if(target_type == entity::BuffTargetType::OTHER) {
    FacadeRequest::GetInstance()->AddBuff(role->GetType(), role->GetID(),
        target_role->GetType(), target_role->GetID(), buff);
  }
}

SkillResultType::type Skill::SkillHitCircle(SkillRole *role) {
  if(role == NULL) {
    return SkillResultType::ERROR_ARGS;
  }

  GameRole *game_role = role->GetGameRole();
  if(game_role == NULL) {
    return SkillResultType::ERROR_ARGS;
  }

  const SkillContext &context = role->GetSkillContext();

  // 获取技能配置
  const SkillCell *cell = Configure::GetInstance()->GetSkill(
      this->GetID(), this->GetLevel());
  if(cell == NULL) {
    return SkillResultType::ERROR_SKILL_ID;
  }

  // 是否为单体范围(目标必然是实体)
  if(cell->range_type_ != RangeType::CIRCLE) {
    return SkillResultType::ERROR_ARGS;
  }

  core::int32 aoe_max_num = cell->aoe_max_target_ +
    this->GetAttribute(entity::SkillVarFields::AOE_MAX_NUM_ADD_VALUE);
  if (aoe_max_num <= 0) {
    return SkillResultType::ERROR_ARGS;
  }

  core::int32 aoe_radius = cell->aoe_radius_ +
    this->GetAttribute(entity::SkillVarFields::AOE_RADIUS_ADD_VALUE);
  if (aoe_radius <= 0) {
    return SkillResultType::ERROR_ARGS;
  }

  // 目标点
  core::uint32 x = 0, y = 0;

  if(cell->target_type_ == TargetType::POSITION ||
      cell->target_type_ == TargetType::POSITION_SELF) {
    // 位置为目标
    x = context.x_;
    y = context.y_;
  } else if(cell->target_type_ == TargetType::ENTITY) {
    // 实体为目标
    SkillRole *dest = SkillRoleManager::GetInstance()->Get(context.target_type_,
        context.target_id_);
    if(dest == NULL) {
      return SkillResultType::ERROR_ARGS;
    }
    GameRole *game_role_dest = dest->GetGameRole();
    if(game_role_dest == NULL) {
      return SkillResultType::ERROR_ARGS;
    }
    game_role_dest->GetPosition(x, y);
  } else {
    return SkillResultType::ERROR_ARGS;
  }

  // 得到目标点的所在圆的对象
  request::RequestSceneSelectTargets request;
  request.__set_type_(game_role->GetType());
  request.__set_id_(game_role->GetID());
  request.__set_area_type_(request::AreaType::AREA_CIRCLE);
  request.__set_max_count_(aoe_max_num);
  request.__set_x_(x);
  request.__set_y_(y);

  request::CircleAreaCondition condition;
  condition.__set_radius_(aoe_radius);

  request.__set_cond_circle_(condition);

  // 设置选择过滤条件
  core::uint32 select_mask = 0;

  // 实体类型
  if(cell->dest_type_ & DestType::NPC) {
    select_mask |= request::SelectMaskType::NPC;
  }
  if(cell->dest_type_ & DestType::ACTOR) {
    select_mask |= request::SelectMaskType::ACTOR;
  }

  // 实体生死状态
  if(cell->status_type_ & StatusType::LIVE) {
    select_mask |= request::SelectMaskType::ALIVE;
  }
  if(cell->status_type_ & StatusType::DEAD) {
    select_mask |= request::SelectMaskType::DEAD;
  }

  // 技能类型
  if(cell->cast_type_ & AffectType::TEAM) {
    select_mask |= request::SelectMaskType::TEAM;
  }
  if(cell->cast_type_ & AffectType::OTHER_TEAM) {
    select_mask |= request::SelectMaskType::OTHER_TEAM;
  }
  if(cell->cast_type_ & AffectType::SELF) {
    select_mask |= request::SelectMaskType::SELF;
  }
  if(cell->cast_type_ & AffectType::OTHER_SELF) {
    select_mask |= request::SelectMaskType::NOT_SELF;
  }

  // 阵营类型
  if(cell->camp_type_ & (1 << (entity::CampRelationType::HOSTLITY-1))) {
    select_mask |= request::SelectMaskType::CAMP_ENEMY;
  }
  if(cell->camp_type_ & (1 << (entity::CampRelationType::FRIENDLY-1))) {
    select_mask |= request::SelectMaskType::CAMP_FRIEND;
  }
  if(cell->camp_type_ & (1 << (entity::CampRelationType::NEUTRAL-1))) {
    select_mask |= request::SelectMaskType::CAMP_NEUTRAL;
  }
  if(cell->camp_type_ & (1 << (entity::CampRelationType::NONE-1))) {
    select_mask |= request::SelectMaskType::CAMP_NONE;
  }

  // 是否需要pk检测
  if(cell->friend_type_ == FriendType::HURT) {
    select_mask |= request::SelectMaskType::PK_CHECK;
  }

  request.__set_select_mask_(select_mask);

  if(ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_SCENE_SELECT_TARGETS,
        &request, sizeof(request)) == -1) {
    return SkillResultType::ERROR_ARGS;
  }

  // 随机数
  core::uint32 rand =
    ExtensionManager::GetInstance()->GetRandom().Random(global::kPercentDenom);

  // 加BUFF请求
  request::RequestBuffStart request_add_buff;
  request_add_buff.__set_src_entity_type_(role->GetType());
  request_add_buff.__set_src_entity_id_(role->GetID());

  bool global_debuff = false;
  // 公共 Debuffs 只有在伤害类型技能中才会触发
  SkillRole::DebuffVector &buffs = role->GetGlobalDebuffs();

  if(cell->hurt_id_ > 0) {
    if(cell->friend_type_ == FriendType::HURT) {
      if(buffs.empty() == false) {
        core::uint32 rand =
          ExtensionManager::GetInstance()->GetRandom().Random(global::kPercentDenom);
        size_t pos = rand % buffs.size();
        if(rand <= buffs[pos].odds_ &&
            FacadeRequest::GetInstance()->CheckCooling(role->GetType(), role->GetID(),
              buffs[pos].cooling_) == false) {
          // 冷却
          FacadeRequest::GetInstance()->Cooling(role->GetType(), role->GetID(),
              buffs[pos].cooling_);
          global_debuff = true;
        }
      }
    }
  }

  // 循环计算伤害
  size_t pos = 0;
  for(; pos < request.targets_.size(); ++pos) {
    SkillRole *target_role = SkillRoleManager::GetInstance()->Get(
        request.targets_[pos].type_, request.targets_[pos].id_);
    if(target_role) {
      // this->HurtCalculate(role, target_role, cell->hurt_id_);
      if(cell->hurt_id_ > 0) {
        DamageCalculation::GetInstance()->HurtCalculate(role, target_role, cell->hurt_id_, cell->hit_type_, this);
      }
      request_add_buff.__set_dest_entity_type_(target_role->GetType());
      request_add_buff.__set_dest_entity_id_(target_role->GetID());
      // 技能BUFF
      std::map<core::uint32 ,core::uint32>::const_iterator iterator = cell->buffs_.begin();
      for(; iterator != cell->buffs_.end(); ++iterator) {
        if(iterator->second >= rand) {
          request_add_buff.__set_buff_id_(iterator->first);
          // 请求发送
          ExtensionManager::GetInstance()->Request(
              request::RequestType::REQUEST_BUFF_START, &request_add_buff,
              sizeof(request_add_buff));
        }
      }
      // 加BUFF
      if(global_debuff) {
        this->AddGlobalBuff(role, target_role, buffs[pos].buff_, (entity::BuffTargetType::type)
            buffs[pos].target_type_);
        this->OnRoleAttacked(target_role, role, cell);
      }
    }
  }
  /*
  // 如果目标是实体类型
  if(cell->target_type_ == TargetType::ENTITY) {
  // 对目标计算伤害
    SkillRole *target_role = SkillRoleManager::GetInstance()->Get(
        context.target_type_, context.target_id_);
    if(target_role) {
      this->HurtCalculate(role, target_role, cell->hurt_id_);
      request_add_buff.__set_dest_entity_type_(target_role->GetType());
      request_add_buff.__set_dest_entity_id_(target_role->GetID());
      // 技能BUFF
      std::map<core::uint32 ,core::uint32>::const_iterator iterator = cell->buffs_.begin();
      for(; iterator != cell->buffs_.end(); ++iterator) {
        if(iterator->second >= rand) {
          request_add_buff.__set_buff_id_(iterator->first);
          // 请求发送
          ExtensionManager::GetInstance()->Request(
              request::RequestType::REQUEST_BUFF_START, &request_add_buff,
              sizeof(request_add_buff));
        }
      }
    }
  }
  */

  return SkillResultType::SUCCESS;
}

void Skill::SkillHitExtraAoe(SkillRole *role) {
  if (role == NULL) {
    LOG_ERROR("参数错误");
    return;
  }

  GameRole *game_role = role->GetGameRole();
  if (game_role == NULL) {
    LOG_ERROR("获取 GameRole 失败");
    return;
  }

  const SkillContext &context = role->GetSkillContext();

  const SkillCell *cell = Configure::GetInstance()->GetSkill(this->GetID(),
      this->GetLevel());
  if (cell == NULL) {
    LOG_ERROR("获取 SkillCell(%d, %d) 失败", this->GetID(), this->GetLevel());
    return;
  }

  if (cell->extra_aoe_max_target_ <= 0) {
    LOG_ERROR("SkillCell(%d,%d) extra_aoe_max_target_(%d) 错误",
        this->GetID(), this->GetLevel(), cell->extra_aoe_max_target_);
    return;
  }

  core::uint32 x = 0, y = 0;
  game_role->GetPosition(x, y);

  // 得到目标点的所在圆的对象
  request::RequestSceneSelectTargets request;
  request.__set_type_(game_role->GetType());
  request.__set_id_(game_role->GetID());
  request.__set_area_type_(request::AreaType::AREA_CIRCLE);
  request.__set_max_count_(cell->extra_aoe_max_target_);
  request.__set_x_(x);
  request.__set_y_(y);

  request::CircleAreaCondition condition;
  condition.__set_radius_(cell->extra_aoe_radius_);

  request.__set_cond_circle_(condition);

  // 设置选择过滤条件
  core::uint32 select_mask = 0;

  // 实体类型
  if(cell->extra_aoe_dest_type_ & DestType::NPC) {
    select_mask |= request::SelectMaskType::NPC;
  }
  if(cell->extra_aoe_dest_type_ & DestType::ACTOR) {
    select_mask |= request::SelectMaskType::ACTOR;
  }

  // 实体生死状态
  select_mask |= request::SelectMaskType::ALIVE;

  // 技能类型
  if(cell->extra_aoe_cast_type_ & AffectType::TEAM) {
    select_mask |= request::SelectMaskType::TEAM;
  }
  if(cell->extra_aoe_cast_type_ & AffectType::OTHER_TEAM) {
    select_mask |= request::SelectMaskType::OTHER_TEAM;
  }
  if(cell->extra_aoe_cast_type_ & AffectType::SELF) {
    select_mask |= request::SelectMaskType::SELF;
  }
  if(cell->extra_aoe_cast_type_ & AffectType::OTHER_SELF) {
    select_mask |= request::SelectMaskType::NOT_SELF;
  }

  // 阵营类型
  if(cell->extra_aoe_camp_type_ & (1 << (entity::CampRelationType::HOSTLITY-1))) {
    select_mask |= request::SelectMaskType::CAMP_ENEMY;
  }
  if(cell->extra_aoe_camp_type_ & (1 << (entity::CampRelationType::FRIENDLY-1))) {
    select_mask |= request::SelectMaskType::CAMP_FRIEND;
  }
  if(cell->extra_aoe_camp_type_ & (1 << (entity::CampRelationType::NEUTRAL-1))) {
    select_mask |= request::SelectMaskType::CAMP_NEUTRAL;
  }
  if(cell->extra_aoe_camp_type_ & (1 << (entity::CampRelationType::NONE-1))) {
    select_mask |= request::SelectMaskType::CAMP_NONE;
  }

  // 是否需要pk检测
  if(cell->extra_aoe_friend_type_ == FriendType::HURT) {
    select_mask |= request::SelectMaskType::PK_CHECK;
  }

  request.__set_select_mask_(select_mask);

  if(ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_SCENE_SELECT_TARGETS,
        &request, sizeof(request)) == -1) {
    return;
  }

  if(cell->extra_aoe_hurt_id_ > 0) {
    // 循环计算伤害
    size_t pos = 0;
    for(; pos < request.targets_.size(); ++pos) {
      SkillRole *target_role = SkillRoleManager::GetInstance()->Get(
          request.targets_[pos].type_, request.targets_[pos].id_);
      if(target_role) {
        // this->HurtCalculate(role, target_role, cell->hurt_id_);
        DamageCalculation::GetInstance()->HurtCalculate(role, target_role, cell->extra_aoe_hurt_id_, cell->hit_type_, this);
        this->OnRoleAttacked(target_role, role, cell);
      }
    }
  }
}

SkillResultType::type Skill::SkillHitSector(SkillRole *role, request::SectorRadianType::type radian) {
  if(role == NULL) {
    return SkillResultType::ERROR_ARGS;
  }

  GameRole *game_role = role->GetGameRole();
  if(game_role == NULL) {
    return SkillResultType::ERROR_ARGS;
  }

  const SkillContext &context = role->GetSkillContext();

  // 获取技能配置
  const SkillCell *cell = Configure::GetInstance()->GetSkill(
      this->GetID(), this->GetLevel());
  if(cell == NULL) {
    return SkillResultType::ERROR_SKILL_ID;
  }

  // 是否为单体范围(目标必然是实体)
  if(cell->range_type_ != RangeType::SECTOR_QUARTER &&
    cell->range_type_ != RangeType::SECTOR_HALF) {
    return SkillResultType::ERROR_ARGS;
  }

  core::int32 aoe_max_num = cell->aoe_max_target_ +
    this->GetAttribute(entity::SkillVarFields::AOE_MAX_NUM_ADD_VALUE);
  if (aoe_max_num <= 0) {
    return SkillResultType::ERROR_ARGS;
  }

  core::int32 aoe_radius = cell->aoe_radius_ +
    this->GetAttribute(entity::SkillVarFields::AOE_RADIUS_ADD_VALUE);
  if (aoe_radius <= 0) {
    return SkillResultType::ERROR_ARGS;
  }

  // 目标点
  core::uint32 x = 0, y = 0;

  if(cell->target_type_ == TargetType::POSITION ||
      cell->target_type_ == TargetType::POSITION_SELF) {
    // 位置为目标
    x = context.x_;
    y = context.y_;
  } else if(cell->target_type_ == TargetType::ENTITY) {
    // 实体为目标
    SkillRole *dest = SkillRoleManager::GetInstance()->Get(context.target_type_,
        context.target_id_);
    if(dest == NULL) {
      return SkillResultType::ERROR_ARGS;
    }
    GameRole *game_role_dest = dest->GetGameRole();
    if(game_role_dest == NULL) {
      return SkillResultType::ERROR_ARGS;
    }
    game_role_dest->GetPosition(x, y);
  } else {
    return SkillResultType::ERROR_ARGS;
  }

  entity::DirectionType::type direction =
    global::DirectionCalculator::GetInstance()->GetGridDirection(game_role->GetPosx(),
        game_role->GetPosy(), x, y);
  if(direction < entity::DirectionType::MIN || direction >= entity::DirectionType::NONE) {
    return SkillResultType::ERROR_DISTANCE;
  }

  request::SectorAreaCondition condition;
  condition.__set_radius_(aoe_radius);
  condition.__set_dir_(direction);
  condition.__set_radian_(radian);

  // 得到目标点的所在扇形对象
  request::RequestSceneSelectTargets request;
  request.__set_type_(game_role->GetType());
  request.__set_id_(game_role->GetID());
  request.__set_area_type_(request::AreaType::AREA_SECTOR_QUARTER);
  request.__set_max_count_(aoe_max_num);
  request.__set_x_(game_role->GetPosx());
  request.__set_y_(game_role->GetPosy());
  request.__set_cond_sector_(condition);

  // 设置选择过滤条件
  core::uint32 select_mask = 0;

  // 实体类型
  if(cell->dest_type_ & DestType::NPC) {
    select_mask |= request::SelectMaskType::NPC;
  }
  if(cell->dest_type_ & DestType::ACTOR) {
    select_mask |= request::SelectMaskType::ACTOR;
  }

  // 实体生死状态
  if(cell->status_type_ & StatusType::LIVE) {
    select_mask |= request::SelectMaskType::ALIVE;
  }
  if(cell->status_type_ & StatusType::DEAD) {
    select_mask |= request::SelectMaskType::DEAD;
  }

  // 技能类型
  if(cell->cast_type_ & AffectType::TEAM) {
    select_mask |= request::SelectMaskType::TEAM;
  }
  if(cell->cast_type_ & AffectType::OTHER_TEAM) {
    select_mask |= request::SelectMaskType::OTHER_TEAM;
  }
  if(cell->cast_type_ & AffectType::SELF) {
    select_mask |= request::SelectMaskType::SELF;
  }
  if(cell->cast_type_ & AffectType::OTHER_SELF) {
    select_mask |= request::SelectMaskType::NOT_SELF;
  }

  // 阵营类型
  if(cell->camp_type_ & (1 << (entity::CampRelationType::HOSTLITY-1))) {
    select_mask |= request::SelectMaskType::CAMP_ENEMY;
  }
  if(cell->camp_type_ & (1 << (entity::CampRelationType::FRIENDLY-1))) {
    select_mask |= request::SelectMaskType::CAMP_FRIEND;
  }
  if(cell->camp_type_ & (1 << (entity::CampRelationType::NEUTRAL-1))) {
    select_mask |= request::SelectMaskType::CAMP_NEUTRAL;
  }
  if(cell->camp_type_ & (1 << (entity::CampRelationType::NONE-1))) {
    select_mask |= request::SelectMaskType::CAMP_NONE;
  }

  // 是否需要pk检测
  if(cell->friend_type_ == FriendType::HURT) {
    select_mask |= request::SelectMaskType::PK_CHECK;
  }

  request.__set_select_mask_(select_mask);

  if(ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_SCENE_SELECT_TARGETS,
        &request, sizeof(request)) == -1) {
    return SkillResultType::ERROR_ARGS;
  }

  // 随机数
  core::uint32 rand =
    ExtensionManager::GetInstance()->GetRandom().Random(global::kPercentDenom);

  // 加BUFF请求
  request::RequestBuffStart request_add_buff;
  request_add_buff.__set_src_entity_type_(role->GetType());
  request_add_buff.__set_src_entity_id_(role->GetID());

  bool global_debuff = false;
  // 公共 Debuffs 只有在伤害类型技能中才会触发
  SkillRole::DebuffVector &debuffs = role->GetGlobalDebuffs();
  if(cell->hurt_id_ > 0) {
    if(cell->friend_type_ == FriendType::HURT) {
      if(debuffs.empty() == false) {
        core::uint32 rand =
          ExtensionManager::GetInstance()->GetRandom().Random(global::kPercentDenom);
        size_t pos = rand % debuffs.size();
        if(rand <= debuffs[pos].odds_ &&
            FacadeRequest::GetInstance()->CheckCooling(role->GetType(), role->GetID(),
              debuffs[pos].cooling_) == false) {
          // 冷却
          FacadeRequest::GetInstance()->Cooling(role->GetType(), role->GetID(),
              debuffs[pos].cooling_);
          global_debuff = true;
        }
      }
    }
  }
  // 循环计算伤害
  size_t pos = 0;
  for(; pos < request.targets_.size(); ++pos) {
    SkillRole *target_role = SkillRoleManager::GetInstance()->Get(
        request.targets_[pos].type_, request.targets_[pos].id_);
    if(target_role) {
      // this->HurtCalculate(role, target_role, cell->hurt_id_);
      if(cell->hurt_id_ > 0) {
        DamageCalculation::GetInstance()->HurtCalculate(role, target_role, cell->hurt_id_, cell->hit_type_, this);
      }
      request_add_buff.__set_dest_entity_type_(target_role->GetType());
      request_add_buff.__set_dest_entity_id_(target_role->GetID());
      // 技能BUFF
      std::map<core::uint32 ,core::uint32>::const_iterator iterator = cell->buffs_.begin();
      for(; iterator != cell->buffs_.end(); ++iterator) {
        if(iterator->second >= rand) {
          request_add_buff.__set_buff_id_(iterator->first);
          // 请求发送
          ExtensionManager::GetInstance()->Request(
              request::RequestType::REQUEST_BUFF_START, &request_add_buff,
              sizeof(request_add_buff));
        }
      }
      // 加BUFF
      if(global_debuff) {
        FacadeRequest::GetInstance()->AddBuff(role->GetType(), role->GetID(),
            target_role->GetType(), target_role->GetID(), debuffs[pos].buff_);
      }
      this->OnRoleAttacked(target_role, role, cell);
    }
  }

  return SkillResultType::SUCCESS;
}

void Skill::OnRoleAttacked(SkillRole *attacked_role, SkillRole *attacker_role, const  SkillCell* cell) {
  if(attacked_role == NULL || attacker_role == NULL || cell == NULL) { 
    LOG_ERROR("参数错误");
    return ;
  }

  GameRole* src_role = attacker_role->GetGameRole();
  GameRole* dest_role = attacked_role->GetGameRole();
  if(src_role == NULL || dest_role == NULL) {
    LOG_ERROR("获取 GameRole 失败");
    return ;
  }

  event::ChannelType::type type = event::ChannelType::CHANNEL_MAX;
  switch(attacked_role->GetType()) {
    case entity::EntityType::TYPE_ACTOR:
      type = event::ChannelType::CHANNEL_ACTOR;
      break;
    case entity::EntityType::TYPE_NPC:
      type = event::ChannelType::CHANNEL_NPC;
      break;
    default:
      break;
  }

  // 广播被技能攻击事件
  event::EventAttackedSkill event;
  event.__set_id_(attacked_role->GetID());
  event.__set_type_(attacked_role->GetType());
  event.__set_skill_id_(cell->id_);
  event.__set_friend_type_(cell->friend_type_);
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_SKILL_ATTACKED_SKILL, attacked_role->GetID(),
      type, &event, sizeof(event::EventAttackedSkill));

  if(src_role->GetType() == entity::EntityType::TYPE_ACTOR &&
      dest_role->GetType() == entity::EntityType::TYPE_ACTOR) {
    // 名字颜色与PK模式变化
    if(cell->friend_type_ == FriendType::HURT) {
      // 取消死亡保护状态
      src_role->RemoveDieProtectBuff();
      // 设置为褐名
      ((GameActor*)src_role)->SetBrownName();
      // 被攻击玩家如果选择的pk模式切换
      core::int32 dest_pk = ((GameActor*)dest_role)->GetAttribute(entity::ActorClientFields::PK_MODE); 
      core::int8 dest_pk_mode = core::int8(dest_pk);
      // 如果是和平模式立刻切换
      if(entity::ActorPKModeType::PEACE == dest_pk_mode) {
        dest_pk_mode = entity::ActorPKModeType::ATTACK;
        core::int8 pkchoose = (core::int8)((dest_pk & 0x0000FF00) >> 8);
        if(entity::ActorPKChooseType::SELF_DEFENSE & pkchoose) {
          dest_pk = (pkchoose << 8) + dest_pk_mode;
          ((GameActor*)dest_role)->SetAttribute(entity::ActorClientFields::PK_MODE, dest_pk);
        }
      }
    } else {
      // 对正在PVP的玩家使用善意技能会让自己进入PVP
      if(attacked_role->GetFightingStatus().GetPvpStatus() == true) {
        attacker_role->GetFightingStatus().SetPvpFighting();
      }
      // 对红名玩家使用善意技能会让自己红名
      if(((GameActor*)dest_role)->GetBrownName() == true) {
        ((GameActor*)src_role)->SetBrownName();
      }
    }
  }

  // 名字颜色与PK模式变化
  if(cell->friend_type_ == FriendType::HURT &&
      src_role->GetType() == entity::EntityType::TYPE_ACTOR &&
      dest_role->GetType() == entity::EntityType::TYPE_ACTOR) {
    // 取消死亡保护状态
    src_role->RemoveDieProtectBuff();
    // 设置为褐名
    ((GameActor*)src_role)->SetBrownName();
    // 被攻击玩家如果选择的pk模式切换
    core::int32 dest_pk = ((GameActor*)dest_role)->GetAttribute(entity::ActorClientFields::PK_MODE); 
    core::int8 dest_pk_mode = core::int8(dest_pk);
    // 如果是和平模式立刻切换
    if(entity::ActorPKModeType::PEACE == dest_pk_mode) {
      dest_pk_mode = entity::ActorPKModeType::ATTACK;
      core::int8 pkchoose = (core::int8)((dest_pk & 0x0000FF00) >> 8);
      if(entity::ActorPKChooseType::SELF_DEFENSE & pkchoose) {
        dest_pk = (pkchoose << 8) + dest_pk_mode;
        ((GameActor*)dest_role)->SetAttribute(entity::ActorClientFields::PK_MODE, dest_pk);
      }
    }
  }
}

core::int32 Skill::GetAttribute(entity::SkillVarFields::type type) const {
  if(type < entity::SkillVarFields::SKILL_END) {
    return this->attributes_[type];
  }
  return 0;
}

bool Skill::SetAttribute(entity::SkillVarFields::type type, core::int32 value,
    bool synchronize) {
  if(type < entity::SkillVarFields::SKILL_END) {
    // 设置属性
    this->attributes_[type] = value;
    if(synchronize) {
      this->Synchronize(type);
    }
    return true;
  }
  return false;
}

void Skill::Synchronize() {
  if (this->role_ == NULL || this->role_->GetGameRole()) {
    LOG_ERROR("role_ 为空");
    return;
  }

  if (this->role_->GetType() == entity::EntityType::TYPE_ACTOR) {
    gateway::protocol::MessageSkillAttributeSynchronize message;
    message.__set_skill_id_(this->GetID());
    entity::SkillVarField field;
    for (size_t pos = 0; pos < this->attributes_.size(); ++pos) {
      field.__set_type_((entity::SkillVarFields::type)pos);
      field.__set_value_(this->GetAttribute((entity::SkillVarFields::type)pos));
      message.attributes_.push_back(field);
    }
    ((GameActor *)this->role_->GetGameRole())->SendMessage(message,
        gateway::protocol::MessageType::MESSAGE_SKILL_ATTRIBUTE_SYNCHRONIZE);
  }
}

void Skill::Synchronize(entity::SkillVarFields::type type) {
  if (this->role_ == NULL || this->role_->GetGameRole() == NULL) {
    LOG_ERROR("role_ 为空");
    return;
  }

  if (this->role_->GetType() == entity::EntityType::TYPE_ACTOR) {
    gateway::protocol::MessageSkillAttributeSynchronize message;
    message.__set_skill_id_(this->GetID());
    entity::SkillVarField field;
    field.__set_type_(type);
    field.__set_value_(this->GetAttribute(type));
    message.attributes_.push_back(field);
    ((GameActor *)this->role_->GetGameRole())->SendMessage(message,
        gateway::protocol::MessageType::MESSAGE_SKILL_ATTRIBUTE_SYNCHRONIZE);
  }
}

}  // namespace skill

}  // namespace server

}  // namespace game

