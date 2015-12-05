//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-11-19 18:51:41.
// File name: request_skill_hurt.cc
//
// Description:
// Define class RequestSkillHurt.
//

#include "game_server/server/extension/skill/request_skill_hurt.h"

#include <boost/bind.hpp>

#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/skill/damage_calculation.h"
#include "game_server/server/extension/skill/skill_role.h"
#include "game_server/server/extension/skill/skill_role_manager.h"
#include "game_server/server/request/game_skill_request_types.h"
#include "game_server/server/request/game_request_types.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace skill {

RequestSkillHurt::RequestSkillHurt() {}
RequestSkillHurt::~RequestSkillHurt() {}

bool RequestSkillHurt::Initialize() {
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_SKILL_HURT,
      boost::bind(&RequestSkillHurt::OnRequest, this, _1, _2));

  return true;
}

void RequestSkillHurt::Finalize() {
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_SKILL_HURT);
}

int RequestSkillHurt::OnRequest(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestSkillHurt) != size) {
    LOG_ERROR("参数错误");
    return -1;
  }

  request::RequestSkillHurt *request = (request::RequestSkillHurt *)message;

  SkillRole *role = SkillRoleManager::GetInstance()->Get(request->type_, request->id_);
  if(role == NULL) {
    LOG_ERROR("获取 SkillRole(%d, %lu) 失败", request->type_, request->id_);
    return -1;
  }

  GameRole *game_role = role->GetGameRole();
  if(game_role == NULL) {
    LOG_ERROR("获取 GameRole(%d, %lu) 失败", request->type_, request->id_);
    return -1;
  }

  if(game_role->GetBasicStatus() == entity::RoleBasicStatus::STATUS_DIE) {
    return 0;
  }

  SkillRole *src_role = SkillRoleManager::GetInstance()->Get(
      request->src_type_, request->src_id_);
  if(src_role == NULL) {
    DamageCalculation::GetInstance()->HurtCalculate(NULL, role, request->hurt_id_,
        SkillHitType::COMMON | SkillHitType::DODGE | SkillHitType::CRIT | SkillHitType::IGNORE_DEFENCE,
        NULL);
  } else {
    DamageCalculation::GetInstance()->HurtCalculate(src_role, role, request->hurt_id_,
        SkillHitType::COMMON | SkillHitType::DODGE | SkillHitType::CRIT | SkillHitType::IGNORE_DEFENCE,
        NULL);
  }

  return 0;
}

}  // namespace skill

}  // namespace server

}  // namespace game

