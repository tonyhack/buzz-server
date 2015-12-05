//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-06-25 17:48:16.
// File name: request_set_fighting_status.cc
//
// Description:
// Define class RequestSetFightingStatus.
//

#include "game_server/server/extension/skill/request_set_fighting_status.h"

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

RequestSetFightingStatus::RequestSetFightingStatus() {}
RequestSetFightingStatus::~RequestSetFightingStatus() {}

bool RequestSetFightingStatus::Initialize() {
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_SKILL_SET_FIGHTING_STATUS,
      boost::bind(&RequestSetFightingStatus::OnRequest, this, _1, _2));

  return true;
}

void RequestSetFightingStatus::Finalize() {
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_SKILL_SET_FIGHTING_STATUS);
}

int RequestSetFightingStatus::OnRequest(void *message, size_t size) {
  if (message == NULL || sizeof(request::RequestSkillSetFightingStatus) != size) {
    LOG_ERROR("参数错误");
    return -1;
  }

  request::RequestSkillSetFightingStatus *request =
    (request::RequestSkillSetFightingStatus *)message;

  SkillRole *role = NULL;
  if (request->type_ == entity::EntityType::TYPE_ACTOR) {
    role = SkillRoleManager::GetInstance()->GetActor(request->id_);
  } else if (request->type_ == entity::EntityType::TYPE_NPC) {
    role = SkillRoleManager::GetInstance()->GetNpc(request->id_);
  } else {
    return -1;
  }

  if (role == NULL) {
    LOG_ERROR("获取 SkillRole(%d,%lu) 失败", request->type_, request->id_);
    return -1;
  }

  if (request->status_ == true) {
    role->GetFightingStatus().SetStatus();
  } else {
    role->GetFightingStatus().ResetStatus(true);
  }

  return 0;
}

}  // namespace skill

}  // namespace server

}  // namespace game
