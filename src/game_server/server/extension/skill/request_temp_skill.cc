//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-02-18 12:20:31.
// File name: request_temp_skill.cc
//
// Description:
// Define class RequestTempSkill.
//

#include "game_server/server/extension//skill/request_temp_skill.h"

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

RequestTempSkill::RequestTempSkill() {}
RequestTempSkill::~RequestTempSkill() {}

bool RequestTempSkill::Initialize() {
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_SKILL_ADD_TEMP_SKILL,
      boost::bind(&RequestTempSkill::OnRequestSkillAddTempSkill, this, _1, _2));

  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_SKILL_REMOVE_TEMP_SKILL,
      boost::bind(&RequestTempSkill::OnRequestSkillRemoveTempSkill, this, _1, _2));

  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_SKILL_CHANGE_SKILL_FORM,
      boost::bind(&RequestTempSkill::OnRequestSkillChangeSkillForm, this, _1, _2));

  return true;
}

void RequestTempSkill::Finalize() {
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_SKILL_ADD_TEMP_SKILL);

  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_SKILL_REMOVE_TEMP_SKILL);

  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_SKILL_CHANGE_SKILL_FORM);
}

int RequestTempSkill::OnRequestSkillAddTempSkill(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestSkillAddTempSkill) != size) {
    LOG_ERROR("参数错误");
    return -1;
  }

  request::RequestSkillAddTempSkill *request = (request::RequestSkillAddTempSkill *)message;

  SkillRole *role = NULL;
  if(request->type_ == entity::EntityType::TYPE_ACTOR) {
    role = SkillRoleManager::GetInstance()->GetActor(request->id_);
  } else if(request->type_ == entity::EntityType::TYPE_NPC) {
    role = SkillRoleManager::GetInstance()->GetNpc(request->id_);
  } else {
    return -1;
  }

  if(role == NULL) {
    LOG_ERROR("获取 SkillRole(%d,%lu) 失败", request->type_, request->id_);
    return -1;
  }

  if(role->AddTempSkill(request->skill_id_, request->skill_level_) == false) {
    LOG_ERROR("AddTempSkill(%d,%d) 失败", request->skill_id_, request->skill_level_);
    return -1;
  }

  request->__set_result_(true);

  return 0;
}

int RequestTempSkill::OnRequestSkillRemoveTempSkill(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestSkillRemoveTempSkill) != size) {
    LOG_ERROR("参数错误");
    return -1;
  }

  request::RequestSkillRemoveTempSkill *request = (request::RequestSkillRemoveTempSkill *)message;

  SkillRole *role = NULL;
  if(request->type_ == entity::EntityType::TYPE_ACTOR) {
    role = SkillRoleManager::GetInstance()->GetActor(request->id_);
  } else if(request->type_ == entity::EntityType::TYPE_NPC) {
    role = SkillRoleManager::GetInstance()->GetNpc(request->id_);
  } else {
    return -1;
  }

  if(role == NULL) {
    LOG_ERROR("获取 SkillRole(%d,%lu) 失败", request->type_, request->id_);
    return -1;
  }

  if(role->RemoveTempSkill(request->skill_id_) == false) {
    LOG_ERROR("RemoveTempSkill(%d) 失败", request->skill_id_);
    return -1;
  }

  request->__set_result_(true);

  return 0;
}

int RequestTempSkill::OnRequestSkillChangeSkillForm(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestSkillChangeSkillForm) != size) {
    LOG_ERROR("参数错误");
    return -1;
  }

  request::RequestSkillChangeSkillForm *request = (request::RequestSkillChangeSkillForm *)message;

  SkillRole *role = NULL;
  if(request->type_ == entity::EntityType::TYPE_ACTOR) {
    role = SkillRoleManager::GetInstance()->GetActor(request->id_);
  } else if(request->type_ == entity::EntityType::TYPE_NPC) {
    role = SkillRoleManager::GetInstance()->GetNpc(request->id_);
  } else {
    return -1;
  }

  if(role == NULL) {
    LOG_ERROR("获取 SkillRole(%d,%lu) 失败", request->type_, request->id_);
    return -1;
  }

  role->SetSkillForm(request->form_);

  request->__set_result_(true);

  return 0;
}

}  // namespace skill

}  // namespace server

}  // namespace game

