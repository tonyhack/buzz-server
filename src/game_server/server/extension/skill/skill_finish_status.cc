//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-05 11:42:51.
// File name: skill_finish_status.cc
//
// Description:
// Define class SkillFinishStatus.
//

#include "game_server/server/extension/skill/skill_finish_status.h"

#include "game_server/server/game_server.h"
#include "game_server/server/extension/skill/skill_configure.h"
#include "game_server/server/extension/skill/skill_role.h"

namespace game {

namespace server {

namespace skill {

SkillFinishStatus::SkillFinishStatus() {}
SkillFinishStatus::~SkillFinishStatus() {}

bool SkillFinishStatus::Initialize(SkillRole *role) {
  return SkillStatus::Initialize(role);
}

void SkillFinishStatus::Finalize() {
  SkillStatus::Finalize();
}

void SkillFinishStatus::OnEnter() {
  if(this->role_ == NULL) {
    global::LogError("%s:%d (%s) 技能完成状态时找不到 role 对象",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // global::LogDebug("[技能流程] 技能转到FINISH状态");

  const SkillContext &context = this->role_->GetSkillContext();

  // 获取技能对象
  Skill *skill = this->role_->GetUseSkill(context.id_);
  if(skill == NULL) {
    global::LogError("%s:%d (%s) ROLE(%u,%lu) 完成状态找不到技能对象(%u)",
        __FILE__, __LINE__, __FUNCTION__, this->role_->GetType(),
        this->role_->GetID(), context.id_);
    this->role_->GotoStatus(SkillStatusType::IDLE);
    return ;
  }

  // 完成技能命中计算
  skill->OnFinishSkill(this->role_);

  // 返回技能待机状态
  this->role_->GotoStatus(SkillStatusType::IDLE);
}

void SkillFinishStatus::OnLeave() {}

int SkillFinishStatus::OnExecute(request::RequestType::type type,
    const void *data, size_t size) {
  return SkillResultType::ERROR_UNKNOWN;
}

}  // namespace skill

}  // namespace server

}  // namespace game

