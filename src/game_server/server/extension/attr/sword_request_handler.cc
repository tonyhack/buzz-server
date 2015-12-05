//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-03-12 02:49:56.
// File name: sword_request_handler.cc
//
// Description:
// Define class SwordRequestHandler.
//

#include "game_server/server/extension/attr/sword_request_handler.h"

#include <boost/bind.hpp>

#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/attr/attr_actor.h"
#include "game_server/server/extension/attr/attr_actor_manager.h"
#include "game_server/server/extension/attr/sword_actor.h"
#include "game_server/server/request/game_attr_request_types.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace attr {

SwordRequestHandler::SwordRequestHandler() {}
SwordRequestHandler::~SwordRequestHandler() {}

bool SwordRequestHandler::Initialize() {
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_ATTR_GET_SWORD_STEP,
      boost::bind(&SwordRequestHandler::OnRequestAttrGetSwordStep, this, _1, _2));

  return true;
}

void SwordRequestHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_ATTR_GET_SWORD_STEP);
}

int SwordRequestHandler::OnRequestAttrGetSwordStep(void *message, size_t size) {
  if (NULL == message || size != sizeof(request::RequestAttrGetSwordStep)) {
    LOG_ERROR("Invalid message");
    return -1;
  }

  request::RequestAttrGetSwordStep *request =
    (request::RequestAttrGetSwordStep *)message;

  // 获取属性玩家对象
  AttrActor *attr_actor = AttrActorManager::GetInstance()->Get(request->actor_);
  if (NULL == attr_actor) {
    LOG_ERROR("Get AttrActor(%lu) from AttrActorManager failed.", request->actor_);
    return -1;
  }

  request->__set_step_(attr_actor->GetSwordActor()->GetCurrentStep());

  return 0;
}

}  // namespace attr

}  // namespace server

}  // namespace game

