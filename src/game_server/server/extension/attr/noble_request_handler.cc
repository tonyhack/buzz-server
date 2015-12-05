//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-03-12 03:01:10.
// File name: noble_request_handler.cc
//
// Description:
// Define class NobleRequestHandler.
//

#include "game_server/server/extension/attr/noble_request_handler.h"

#include <boost/bind.hpp>

#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/attr/attr_actor.h"
#include "game_server/server/extension/attr/attr_actor_manager.h"
#include "game_server/server/extension/attr/noble_actor.h"
#include "game_server/server/request/game_attr_request_types.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace attr {

NobleRequestHandler::NobleRequestHandler() {}
NobleRequestHandler::~NobleRequestHandler() {}

bool NobleRequestHandler::Initialize() {
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_ATTR_GET_NOBLE_LEVEL,
      boost::bind(&NobleRequestHandler::OnRequestAttrGetNobleLevel, this, _1, _2));

  return true;
}

void NobleRequestHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_ATTR_GET_NOBLE_LEVEL);
}

int NobleRequestHandler::OnRequestAttrGetNobleLevel(void *message, size_t size) {
  if (NULL == message || size != sizeof(request::RequestAttrGetNobleLevel)) {
    LOG_ERROR("Invalid params.");
    return -1;
  }

  request::RequestAttrGetNobleLevel *request =
    (request::RequestAttrGetNobleLevel *)message;

  // 获取属性玩家对象
  AttrActor *attr_actor = AttrActorManager::GetInstance()->Get(request->actor_);
  if (NULL == attr_actor) {
    LOG_ERROR("Get AttrActor(%lu) from AttrActorManager failed.", request->actor_);
    return -1;
  }

  request->__set_level_(attr_actor->GetNobleActor()->GetCurrentLevel());

  return 0;
}

}  // namespace attr

}  // namespace server

}  // namespace game

