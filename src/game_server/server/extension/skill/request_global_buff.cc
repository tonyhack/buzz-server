//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-02-11 03:04:01.
// File name: request_global_buff.cc
//
// Description:
// Define class RequestGlobalBuff.
//

#include "game_server/server/extension/skill/request_global_buff.h"

#include <boost/bind.hpp>

#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/skill/skill_role.h"
#include "game_server/server/extension/skill/skill_role_manager.h"
#include "game_server/server/request/game_skill_request_types.h"
#include "game_server/server/request/game_request_types.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace skill {

RequestGlobalBuff::RequestGlobalBuff() {}
RequestGlobalBuff::~RequestGlobalBuff() {}

bool RequestGlobalBuff::Initialize() {
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_ADD_SKILL_GLOBAL_BUFF,
      boost::bind(&RequestGlobalBuff::OnRequestAddGlobalBuff, this, _1, _2));

  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_REMOVE_SKILL_GLOBAL_BUFF,
      boost::bind(&RequestGlobalBuff::OnRequestRemoveGlobalBuff, this, _1, _2));

  return true;
}

void RequestGlobalBuff::Finalize() {
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_ADD_SKILL_GLOBAL_BUFF);

  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_REMOVE_SKILL_GLOBAL_BUFF);
}

int RequestGlobalBuff::OnRequestAddGlobalBuff(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestAddSkillGlobalBuff) != size) {
    LOG_ERROR("参数错误");
    return -1;
  }

  request::RequestAddSkillGlobalBuff *request =
    (request::RequestAddSkillGlobalBuff *)message;

  // 获取 SkillRole 对象
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

  size_t pos = 0;
  for(; pos < request->buffs_.size(); ++pos) {
    role->AddGlobalDebuff(request->buffs_[pos].buff_,
        request->buffs_[pos].odds_, request->buffs_[pos].cooling_, 
        request->buffs_[pos].target_type_);
  }

  return 0;
}

int RequestGlobalBuff::OnRequestRemoveGlobalBuff(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestRemoveSkillGlobalBuff) != size) {
    LOG_ERROR("参数错误");
    return -1;
  }

  request::RequestRemoveSkillGlobalBuff *request =
    (request::RequestRemoveSkillGlobalBuff *)message;

  // 获取 SkillRole 对象
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

  size_t pos = 0;
  for(; pos < request->buffs_.size(); ++pos) {
    role->RemoveGlobalDebuff(request->buffs_[pos].buff_,
        request->buffs_[pos].odds_, request->buffs_[pos].cooling_);
  }

  return 0;
}

}  // namespace skill

}  // namespace server

}  // namespace game

