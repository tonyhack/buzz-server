//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-04 19:08:22.
// File name: skill_boot_status.cc
//
// Description:
// Define class SkillBootStatus.
//

#include "game_server/server/extension/skill/skill_boot_status.h"

#include "game_server/server/game_server.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/extension/skill/configure.h"
#include "game_server/server/extension/skill/skill_configure.h"
#include "game_server/server/extension/skill/skill_role.h"
#include "game_server/server/extension/skill/skill_types.h"
#include "game_server/server/event/game_role_event_types.h"
#include "game_server/server/request/game_request_types.h"
#include "game_server/server/request/game_effect_request_types.h"
#include "gateway_server/protocol/gateway_skill_protocol_types.h"
#include "global/common_functions.h"

namespace game {

namespace server {

namespace skill {

SkillBootStatus::SkillBootStatus() {}
SkillBootStatus::~SkillBootStatus() {}

bool SkillBootStatus::Initialize(SkillRole *role) {
  return SkillStatus::Initialize(role);
}

void SkillBootStatus::Finalize() {
  SkillStatus::Finalize();
  GameServerSingleton::GetInstance().RemoveTimer(0, this);
}

void SkillBootStatus::OnEnter() {
  if(this->role_ == NULL) {
    global::LogError("%s:%d (%s) 技能引导状态时找不到 role 对象",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // global::LogDebug("[技能流程] 技能转到BOOT状态");

  // 获取技能信息
  const SkillContext &context = this->role_->GetSkillContext();

  // 获取技能对象
  Skill *skill = this->role_->GetUseSkill(context.id_);
  if(skill == NULL) {
    global::LogError("%s:%d (%s) ROLE(%u, %lu) 引导状态找不到技能对象(%u)",
        __FILE__, __LINE__, __FUNCTION__, this->role_->GetType(),
        this->role_->GetID(), context.id_);
    this->role_->GotoStatus(SkillStatusType::IDLE);
    return ;
  }

  // 获取技能配置
  const SkillCell *cell = Configure::GetInstance()->GetSkill(
      skill->GetID(), skill->GetLevel());
  if(cell == NULL) {
    global::LogError("%s:%d (%s) 技能(%u, %u) 配置不存在",
        __FILE__, __LINE__, __FUNCTION__, skill->GetID(), skill->GetLevel());
    this->role_->GotoStatus(SkillStatusType::IDLE);
    return ;
  }

  core::int32 need_sing_time = cell->sing_time_ +
    skill->GetAttribute(entity::SkillVarFields::SING_TIME_ADD_VALUE);
  if (need_sing_time < 0) {
    need_sing_time = 0;
  }

  if(need_sing_time < SkillBootStatus::kBootMinMsecs_) {
    this->role_->GotoStatus(SkillStatusType::LAUNCH);
  } else {
    // 广播引导消息
    gateway::protocol::MessageSkillOperate message;
    message.__set_skill_id_(skill->GetID());
    message.__set_type_(gateway::protocol::SkillOperateType::BOOT);
    message.context_.__set_src_type_(this->role_->GetType());
    message.context_.__set_src_id_(global::ToString(this->role_->GetID()));
    if(cell->target_type_ == TargetType::ENTITY) {
      message.context_.__set_target_type_(context.target_type_);
      message.context_.__set_target_id_(global::ToString(context.target_id_));
    } else if(cell->target_type_ == TargetType::POSITION ||
        cell->target_type_ == TargetType::POSITION_SELF) {
      message.context_.__set_x_(context.x_);
      message.context_.__set_y_(context.y_);
    } else {
      message.context_.__set_direction_(context.direction_);
    }
    // global::LogDebug("[技能流程] 广播技能引导消息到客户端");
    // 广播
    this->role_->SendMessageAround(message,
        gateway::protocol::MessageType::MESSAGE_SKILL_OPERATE, true);
    // 启动定时器
    GameServerSingleton::GetInstance().AddTimer(0, need_sing_time, 1,
        this, "SkillBootStatus::OnEnter");
  }

  // 处理角色状态
  if(cell->move_ == SkillMoveType::NO_MOVE) {
    GameRole *game_role = this->role_->GetGameRole();
    if(game_role == NULL) {
      LOG_ERROR("角色没有找到[%lu]", this->role_->GetID());
      return ;
    }
    this->RoleForbidMove(game_role, true);
  }
}

void SkillBootStatus::OnLeave() {
  GameServerSingleton::GetInstance().RemoveTimer(0, this);
  if(this->role_ == NULL) {
    return ;
  }

  // 获取技能信息
  const SkillContext &context = this->role_->GetSkillContext();

  // 获取技能对象
  Skill *skill = this->role_->GetUseSkill(context.id_);
  if(skill == NULL) {
    global::LogError("%s:%d (%s) ROLE(%u, %lu) 引导状态找不到技能对象(%u)",
        __FILE__, __LINE__, __FUNCTION__, this->role_->GetType(),
        this->role_->GetID(), context.id_);
    this->role_->GotoStatus(SkillStatusType::IDLE);
    return ;
  }

  // 获取技能配置
  const SkillCell *cell = Configure::GetInstance()->GetSkill(
      skill->GetID(), skill->GetLevel());
  if(cell == NULL) {
    global::LogError("%s:%d (%s) 技能(%u, %u) 配置不存在",
        __FILE__, __LINE__, __FUNCTION__, skill->GetID(), skill->GetLevel());
    this->role_->GotoStatus(SkillStatusType::IDLE);
    return ;
  }

  // 处理角色状态
  if(cell->move_ == SkillMoveType::NO_MOVE) {
    GameRole *game_role = this->role_->GetGameRole();
    if(game_role == NULL) {
      LOG_ERROR("角色没有找到[%lu]", this->role_->GetID());
      return ;
    }
    this->RoleForbidMove(game_role, false);
  }
}

int SkillBootStatus::OnExecute(request::RequestType::type type,
    const void *data, size_t size) {
  return SkillResultType::ERROR_UNKNOWN;
}

void SkillBootStatus::OnTimer(core::uint32 id) {
  this->role_->GotoStatus(SkillStatusType::LAUNCH);
}

void SkillBootStatus::OnEvent(event::EventType::type type,
    const void *message, size_t size) {
  if(this->role_ == NULL) {
    LOG_ERROR("角色没有找到");
    return ;
  }
  switch(type) {
    case event::EventType::EVENT_SCENE_ROLE_CHANGE_LOCATION:
      this->BreakSynchronize();
      this->role_->GotoStatus(SkillStatusType::IDLE);
      break;
    case event::EventType::EVENT_BASIC_STATUS_CHANGE:
      this->BreakSynchronize();
      this->role_->GotoStatus(SkillStatusType::IDLE);
      break;
    case event::EventType::EVENT_ROLE_COMMON_STATUS_CHANGE:
      this->OnEventRoleCommonStatusChange(message, size);
      break;
    default:
      break;
  }
}

void SkillBootStatus::BreakSynchronize() {
  // 得到GameActor
  if(this->role_ == NULL) {
    global::LogError("%s:%d (%s) 技能引导状态时找不到 role 对象",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  GameRole *game_role = this->role_->GetGameRole();
  if(game_role == NULL) {
    LOG_ERROR("角色没有找到[%lu]", this->role_->GetID());
    return ;
  }
  // 获取技能信息
  const SkillContext &context = this->role_->GetSkillContext();
  if(game_role->GetType() == entity::EntityType::TYPE_ACTOR) {
    GameActor *game_actor = (GameActor *)game_role;
    gateway::protocol::MessageSkillBreakSynchronize message;
    message.__set_skill_id_((core::int32)context.id_);
    message.__set_role_type_(game_role->GetType());
    message.__set_role_id_(global::ToString(game_role->GetID()));
    game_actor->SendMessageAround(message,
        gateway::protocol::MessageType::MEESAGE_SKILL_BREAK_SYNCHRONIZE, true);
  }
}

void SkillBootStatus::RoleForbidMove(GameRole *game_role, bool forbid) {
  if(game_role == NULL) {
    LOG_ERROR("角色没有找到");
    return ;
  }

  // 更改角色的状态
  if(forbid == false) {
    request::RequestEffectCommonStatusEnd effect_end;
    effect_end.__set_entity_type_(game_role->GetType());
    effect_end.__set_entity_id_(game_role->GetID());
    effect_end.__set_status_(entity::RoleCommonStatus::FORBID_MOVE);
    if(ExtensionManager::GetInstance()->Request(
          request::RequestType::REQUEST_EFFECT_COMMON_STATUS_END,
          &effect_end, sizeof(effect_end)) != 0) {
      LOG_ERROR("请求 REQUEST_EFFECT_COMMON_STATUS_END 失败");
      return ;
    }
  } else {
    request::RequestEffectCommonStatusStart effect_start;
    effect_start.__set_entity_type_(game_role->GetType());
    effect_start.__set_entity_id_(game_role->GetID());
    effect_start.__set_status_(entity::RoleCommonStatus::FORBID_MOVE);
    if(ExtensionManager::GetInstance()->Request(
          request::RequestType::REQUEST_EFFECT_COMMON_STATUS_START,
          &effect_start, sizeof(effect_start)) != 0) {
      LOG_ERROR("请求 REQUEST_EFFECT_COMMON_STATUS_START 失败");
      return ;
    }
  }
}

void SkillBootStatus::OnEventRoleCommonStatusChange(const void *message,
    size_t size) {
  if(this->role_ == NULL) {
    LOG_ERROR("玩家对象没有找到");
    return ;
  }
  GameRole *role = this->role_->GetGameRole();
  if(role == NULL) {
    LOG_ERROR("角色对象没有找到");
    return ;
  }
  if(sizeof(event::EventBasicStatusChange) != size) {
    LOG_ERROR("解析长度错误");
    return ;
  }
  event::EventBasicStatusChange *event = 
    (event::EventBasicStatusChange *)message;
  if((event->from_ & entity::RoleCommonStatus::FORBID_USE_SKILL) == 0 &&
      (event->to_ & entity::RoleCommonStatus::FORBID_USE_SKILL) != 0) {
    this->BreakSynchronize();
    this->role_->GotoStatus(SkillStatusType::IDLE);
  }
}

}  // namespace skill

}  // namespace server

}  // namespace game

