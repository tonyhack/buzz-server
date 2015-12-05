//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-24 14:57:29.
// File name: request_stand_skill.cc
//
// Description:
// Define class RequestStandSkill.
//

#include "game_server/server/extension/skill/request_stand_skill.h"

#include <boost/bind.hpp>

#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/skill/configure.h"
#include "game_server/server/extension/skill/skill_configure.h"
#include "game_server/server/extension/skill/skill_pool.h"
#include "game_server/server/extension/skill/skill_role.h"
#include "game_server/server/extension/skill/skill_role_manager.h"
#include "game_server/server/request/game_skill_request_types.h"
#include "game_server/server/request/game_request_types.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace skill {

RequestStandSkill::RequestStandSkill() {}
RequestStandSkill::~RequestStandSkill() {}

bool RequestStandSkill::Initialize() {
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_SKILL_STAND,
      boost::bind(&RequestStandSkill::OnRequestStandSkill, this, _1, _2));

  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_SKILL_SIT,
      boost::bind(&RequestStandSkill::OnRequestSitSkill, this, _1, _2));

  return true;
}

void RequestStandSkill::Finalize() {
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_SKILL_STAND);
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_SKILL_SIT);
}

int RequestStandSkill::OnRequestStandSkill(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestSkillStand) != size) {
    global::LogError("%s:%d (%s) 请求启用技能时，参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  request::RequestSkillStand *request = (request::RequestSkillStand *)message;

  // 获取 SkillRole 对象
  SkillRole *role = NULL;

  if(request->type_ == entity::EntityType::TYPE_ACTOR) {
    role = SkillRoleManager::GetInstance()->GetActor(request->id_);
  } else if(request->type_ == entity::EntityType::TYPE_NPC) {
    role = SkillRoleManager::GetInstance()->GetNpc(request->id_);
  } else {
    return -1;
  }

  // 检测是否存在
  if(role == NULL) {
    global::LogError("%s:%d (%s) 请求启用技能时，Role(%u, %lu) 不存在",
        __FILE__, __LINE__, __FUNCTION__, request->type_, request->id_);
    return -1;
  }

  if(role->SkillStand(request->skill_id_) == false) {
    global::LogError("%s:%d (%s) 请求启用技能时，失败",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  request->__set_result_(SkillResultType::SUCCESS);

  return 0;
}

int RequestStandSkill::OnRequestSitSkill(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestSkillSit) != size) {
    global::LogError("%s:%d (%s) 请求收回技能时，参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  request::RequestSkillSit *request = (request::RequestSkillSit *)message;

  // 获取 SkillRole 对象
  SkillRole *role = NULL;

  if(request->type_ == entity::EntityType::TYPE_ACTOR) {
    role = SkillRoleManager::GetInstance()->GetActor(request->id_);
  } else if(request->type_ == entity::EntityType::TYPE_NPC) {
    role = SkillRoleManager::GetInstance()->GetNpc(request->id_);
  } else {
    return -1;
  }

  // 检测是否存在
  if(role == NULL) {
    global::LogError("%s:%d (%s) 请求收回技能时，Role(%u, %lu) 不存在",
        __FILE__, __LINE__, __FUNCTION__, request->type_, request->id_);
    return -1;
  }

  if(role->SkillSit(request->skill_id_) == false) {
    global::LogError("%s:%d (%s) 请求收回技能时失败",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  request->__set_result_(SkillResultType::SUCCESS);

  return 0;
}

}  // namespace skill

}  // namespace server

}  // namespace game

