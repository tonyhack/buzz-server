#include "game_server/server/extension/attr/spirit_request_handler.h"

#include <boost/bind.hpp>

#include "global/logging.h"
#include "game_server/server/request/game_attr_request_types.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/attr/attr_actor.h"
#include "game_server/server/extension/attr/attr_actor_manager.h"
#include "game_server/server/extension/attr/spirit_actor.h"

namespace game {

namespace server {

namespace attr {

SpiritRequestHandler::SpiritRequestHandler() {}
SpiritRequestHandler::~SpiritRequestHandler() {}

bool SpiritRequestHandler::Initialize() {
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_ATTR_ADD_SPIRIT_FACADE,
      boost::bind(&SpiritRequestHandler::OnRequestAttrAddSpiritFacade, this, _1, _2));

  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_ATTR_GET_SPIRIT_LEVEL,
      boost::bind(&SpiritRequestHandler::OnRequestAttrGetSpiritLevel, this, _1, _2));

  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_ATTR_GET_SPIRIT_FACADE_TYPE,
      boost::bind(&SpiritRequestHandler::OnRequestAttrGetSpiritFacadeType, this, _1, _2));

  return true;
}

void SpiritRequestHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_ATTR_ADD_SPIRIT_FACADE);

  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_ATTR_GET_SPIRIT_LEVEL);

  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_ATTR_GET_SPIRIT_FACADE_TYPE);
}

int SpiritRequestHandler::OnRequestAttrAddSpiritFacade(void *message, size_t size) {
  if (NULL == message || size != sizeof(request::RequestAttrAddSpiritFacade)) {
    LOG_ERROR("Invalid params.");
    return -1;
  }

  request::RequestAttrAddSpiritFacade *request =
    (request::RequestAttrAddSpiritFacade *)message;

  // 获取属性玩家对象
  AttrActor *attr_actor = AttrActorManager::GetInstance()->Get(request->actor_);
  if (NULL == attr_actor) {
    LOG_ERROR("Get AttrActor(%lu) from AttrActorManager failed.", request->actor_);
    return -1;
  }

  bool result = attr_actor->GetSpiritActor()->AddFacade(request->spirit_facade_id_,
      request->duration_time_);

  request->__set_result_(result);

  return 0;
}

int SpiritRequestHandler::OnRequestAttrGetSpiritLevel(void *message, size_t size) {
  if (NULL == message || size != sizeof(request::RequestAttrGetSpiritLevel)) {
    LOG_ERROR("Invalid params.");
    return -1;
  }

  request::RequestAttrGetSpiritLevel *request =
    (request::RequestAttrGetSpiritLevel *)message;

  // 获取属性玩家对象
  AttrActor *attr_actor = AttrActorManager::GetInstance()->Get(request->actor_);
  if (NULL == attr_actor) {
    LOG_ERROR("Get AttrActor(%lu) from AttrActorManager failed.", request->actor_);
    return -1;
  }

  request->__set_level_(attr_actor->GetSpiritActor()->GetCurrentLevel());

  return 0;
}

int SpiritRequestHandler::OnRequestAttrGetSpiritFacadeType(void *message, size_t size) {
  request::RequestAttrGetSpiritFacadeType *request =
    (request::RequestAttrGetSpiritFacadeType *)message;

  // 获取属性玩家对象
  AttrActor *attr_actor = AttrActorManager::GetInstance()->Get(request->actor_);
  if (NULL == attr_actor) {
    LOG_ERROR("Get AttrActor(%lu) from AttrActorManager failed.", request->actor_);
    return -1;
  }

  request->__set_type_(attr_actor->GetSpiritActor()->GetCurrentFacadeType());

  return 0;

}

}  // namespace attr

}  // namespace server

}  // namespace game

