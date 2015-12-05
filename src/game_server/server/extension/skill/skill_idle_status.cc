//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-04 14:26:25.
// File name: skill_idle_status.cc
//
// Description:
// Define class SkillIdleStatus.
//

#include "game_server/server/extension/skill/skill_idle_status.h"

#include "game_server/server/configure.h"
#include "game_server/server/game_npc.h"
#include "game_server/server/npc_configure.h"
#include "game_server/server/extension/skill/configure.h"
#include "game_server/server/extension/skill/skill_role.h"
#include "game_server/server/event/game_skill_event_types.h"
#include "game_server/server/request/game_skill_request_types.h"
#include "game_server/server/event/game_skill_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace skill {

SkillIdleStatus::SkillIdleStatus() {}
SkillIdleStatus::~SkillIdleStatus() {}

bool SkillIdleStatus::Initialize(SkillRole *role) {
  return SkillStatus::Initialize(role);
}

void SkillIdleStatus::Finalize() {
  SkillStatus::Finalize();
}

void SkillIdleStatus::OnEnter() {
  // 请求技能信息
  if(this->role_ == NULL) {
    LOG_ERROR("技能对象为空");
    return ;
  }
  // 获取技能信息
  SkillContext &skill = this->role_->GetSkillContext();
  skill.Clear();
}

void SkillIdleStatus::OnLeave() {
}

int SkillIdleStatus::OnExecute(request::RequestType::type type,
    const void *data, size_t size) {
  switch(type) {
    case request::RequestType::REQUEST_USE_SKILL:
      return this->OnUseSkill(data, size);
    default:
      break;
  }

  return SkillResultType::ERROR_UNKNOWN;
}

int SkillIdleStatus::OnUseSkill(const void *data, size_t size) {
  if(this->role_ == NULL || data == NULL ||
      size != sizeof(request::RequestUseSkill)) {
    global::LogError("%s:%d (%s) 使用技能时参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return SkillResultType::ERROR_ARGS;
  }

  request::RequestUseSkill *request = (request::RequestUseSkill *)data;

  // global::LogDebug("[技能流程] 接收到释放技能(%u)请求", request->skill_id_);

  // 设置技能信息
  SkillContext &context = this->role_->GetSkillContext();
  context.id_ = request->skill_id_;
  context.target_type_ = request->target_type_;
  context.target_id_ = request->target_id_;
  context.x_ = request->x_;
  context.y_ = request->y_;
  context.direction_ = request->direction_;

  // 技能ID为0且角色类型为NPC，则随机一个技能
  if(context.id_ == 0 && this->role_->GetType() ==
      entity::EntityType::TYPE_NPC) {
    GameNpc *npc = (GameNpc *)this->role_->GetGameRole();
    if(npc == NULL) {
      return SkillResultType::ERROR_ARGS;
    }
    const NpcCell *npc_cell = 
      game::server::Configure::GetInstance()->GetNpcConfigure().GetNpc(
          npc->GetTemplateID());
    if(npc_cell == NULL) {
      return SkillResultType::ERROR_ARGS;
    }
    if(npc_cell->skills_.empty()) {
      return SkillResultType::ERROR_ARGS;
    }
    size_t pos =
      ExtensionManager::GetInstance()->GetRandom().Random(npc_cell->skills_.size());
    context.id_ = npc_cell->skills_[pos];
  }

  // 得到技能对象
  Skill *skill = this->role_->GetUseSkill(context.id_);
  if(skill == NULL) {
    global::LogError("%s:%d (%s) 使用技能时 Role 不存在技能ID(%u)",
        __FILE__, __LINE__, __FUNCTION__, context.id_);
    return SkillResultType::ERROR_SKILL_LEVEL;
  }

  // 获取技能配置
  const SkillCell *cell = Configure::GetInstance()->GetSkill(
      skill->GetID(), skill->GetLevel());
  if(cell == NULL) {
    global::LogError("%s:%d (%s) 使用技能时 配置中不存在技能ID(%u)",
        __FILE__, __LINE__, __FUNCTION__, context.id_);
    return SkillResultType::ERROR_SKILL_ID;
  }

  // 检测技能是否满足释放条件
  SkillResultType::type result = skill->CheckUseSkill(this->role_);
  if(result != SkillResultType::SUCCESS) {
    return result;
  }

  // 根据是否吟唱转换状态
  if(cell->sing_time_ > 0) {
    this->role_->GotoStatus(SkillStatusType::BOOT);
  } else {
    this->role_->GotoStatus(SkillStatusType::LAUNCH);
  }
  
  event::ChannelType::type channel = event::ChannelType::CHANNEL_MAX;
  switch(this->role_->GetType()) {
    case entity::EntityType::TYPE_NPC:
      channel = event::ChannelType::CHANNEL_NPC;
        break;
    case entity::EntityType::TYPE_ACTOR:
      channel = event::ChannelType::CHANNEL_ACTOR;
      break;
    default:
      break;
  }
  // 玩家使用技能事件
  event::EventUseSkill use_skill;
  use_skill.__set_type_(request->type_);
  use_skill.__set_id_(request->id_);
  use_skill.__set_skill_id_(request->skill_id_);
  use_skill.__set_skill_group_(cell->group_);
  use_skill.__set_skill_type_(cell->classify_type_);
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_SKILL_USE_SKILL, request->id_,
      channel,&use_skill, sizeof(event::EventUseSkill));

  return SkillResultType::SUCCESS;
}

}  // namespace skill

}  // namespace server

}  // namespace game

