//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-05 10:15:13.
// File name: skill_launch_status.cc
//
// Description:
// Define class SkillLaunchStatus.
//

#include "game_server/server/extension/skill/skill_launch_status.h"

#include "game_server/server/game_server.h"
#include "game_server/server/extension/skill/configure.h"
#include "game_server/server/extension/skill/skill_configure.h"
#include "game_server/server/extension/skill/skill_role.h"
#include "game_server/server/extension/skill/skill.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace skill {

SkillLaunchStatus::SkillLaunchStatus() {}
SkillLaunchStatus::~SkillLaunchStatus() {}

bool SkillLaunchStatus::Initialize(SkillRole *role) {
  return SkillStatus::Initialize(role);
}

void SkillLaunchStatus::Finalize() {
  SkillStatus::Finalize();
  GameServerSingleton::GetInstance().RemoveTimer(0, this);
}

void SkillLaunchStatus::OnEnter() {
  if(this->role_ == NULL) {
    global::LogError("%s:%d (%s) 技能发射状态时找不到 role 对象",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // global::LogDebug("[技能流程] 技能转到LAUNCH状态");

  // 获取技能信息
  const SkillContext &context = this->role_->GetSkillContext();

  // 得到技能对象
  Skill *skill = this->role_->GetUseSkill(context.id_);
  if(skill == NULL) {
    global::LogError("%s:%d (%s) 发射技能时 Role 不存在技能ID(%u)",
        __FILE__, __LINE__, __FUNCTION__, context.id_);
    this->role_->GotoStatus(SkillStatusType::IDLE);
    return ;
  }

  // 得到技能配置
  const SkillCell *skill_cell = Configure::GetInstance()->GetSkill(
      skill->GetID(), skill->GetLevel());
  if(skill_cell == NULL) {
    global::LogError("%s:%d (%s) 发射技能时 技能ID(%u) 不存在",
        __FILE__, __LINE__, __FUNCTION__, context.id_);
    this->role_->GotoStatus(SkillStatusType::IDLE);
    return ;
  }

  // 技能发射
  SkillResultType::type result = skill->OnLaunchSkill(this->role_);
  if(result != SkillResultType::SUCCESS) {
    global::LogError("%s:%d (%s) 技能(%u) 发射失败(%u)",
        __FILE__, __LINE__, __FUNCTION__, context.id_, result);
    this->role_->GotoStatus(SkillStatusType::IDLE);
    return ;
  }

  // 获取与目标的距离
  core::uint32 distance = this->role_->GetSkillDistance(skill_cell);
  if(distance == core::kuint32max) {
    this->role_->GotoStatus(SkillStatusType::IDLE);
    return ;
  }

  if(skill_cell->type_ == SkillType::COLLISION) {
    // 碰撞类技能进入飞行状态
    this->role_->GotoStatus(SkillStatusType::FLIGHT);
  } else {
    // 非碰撞类开启定时器
    core::uint32 flight_time = skill_cell->speed_ * distance;
    if(flight_time > SkillLaunchStatus::kLaunchMinMsecs_) {
      GameServerSingleton::GetInstance().AddTimer(0, flight_time, 1,
          this, "SkillLaunchStatus::OnEnter");
    } else {
      // 如果定时时间过小，直接命中
      this->role_->GotoStatus(SkillStatusType::FINISH);
    }
  }
}

void SkillLaunchStatus::OnLeave() {
  GameServerSingleton::GetInstance().RemoveTimer(0, this);
}

int SkillLaunchStatus::OnExecute(request::RequestType::type type,
    const void *data, size_t size) {
  return SkillResultType::ERROR_UNKNOWN;
}

void SkillLaunchStatus::OnTimer(core::uint32 id) {
  this->role_->GotoStatus(SkillStatusType::FINISH);
}

}  // namespace skill

}  // namespace server

}  // namespace game

