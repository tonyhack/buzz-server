//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-08-01 18:15:50.
// File name: facade_request.cc
//
// Description:
// Define class FacadeRequest.
//

#include "game_server/server/extension/item/facade_request.h"

#include "game_server/server/extension_manager.h"
#include "game_server/server/request/game_request_types.h"
#include "game_server/server/request/game_cooling_request_types.h"
#include "game_server/server/request/game_effect_request_types.h"

namespace game {

namespace server {

namespace item {

bool FacadeRequest::CheckCooling(entity::EntityType::type type, core::uint64 id,
    core::uint32 cooling) {
  request::RequestCheckInCooling request;
  request.__set_type_(type);
  request.__set_id_(id);
  request.__set_cooling_id_(cooling);
  if(ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_CHECK_IN_COOLING, &request,
        sizeof(request)) == 0 && request.in_cooling_ == true) {
    return true;
  }
  return false;
}

void FacadeRequest::StartCooling(entity::EntityType::type type, core::uint64 id,
    core::uint32 cooling) {
  request::RequestStartCooling request;
  request.__set_type_(type);
  request.__set_id_(id);
  request.__set_cooling_id_(cooling);
  ExtensionManager::GetInstance()->Request(
      request::RequestType::REQUEST_START_COOLING, &request,
      sizeof(request));
}

void FacadeRequest::StartEffect(entity::EntityType::type type, core::uint64 id, core::int32 effect_id) {
  request::RequestEffectStart request;
  request.__set_entity_type_(type);
  request.__set_entity_id_(id);
  request.__set_effect_id_(effect_id);
  ExtensionManager::GetInstance()->Request(
      request::RequestType::REQUEST_EFFECT_START, &request,
      sizeof(request));
}

void FacadeRequest::StopEffect(entity::EntityType::type type, core::uint64 id, core::int32 effect_id) {
  request::RequestEffectEnd request;
  request.__set_entity_type_(type);
  request.__set_entity_id_(id);
  request.__set_effect_id_(effect_id);
  ExtensionManager::GetInstance()->Request(
      request::RequestType::REQUEST_EFFECT_END, &request,
      sizeof(request));
}

}  // namespace item

}  // namespace server

}  // namespace game

