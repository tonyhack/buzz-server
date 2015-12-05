#include "game_server/server/extension/chat/facade_request.h"

#include "game_server/server/extension_manager.h"
#include "game_server/server/request/game_request_types.h"
#include "game_server/server/request/game_item_request_types.h"

namespace game {

namespace server {

namespace chat {

bool FacadeRequest::CheckPacketItemExist(core::uint64 actor,
    core::uint32 item_template, core::uint32 number, BindType::type bind) {
  request::RequestGetPacketItemNumber request;
  request.__set_actor_id_(actor);
  request.__set_item_template_(item_template);
  request.__set_bind_((entity::BindRequireType::type)bind);
  if(ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_GET_PAKCET_ITEM_NUMBER, &request,
        sizeof(request)) == 0 && (core::uint32)request.number_ >= number) {
    return true;
  }
  return false;
}

core::uint32 FacadeRequest::RemovePacketItem(core::uint64 actor, core::uint32 item_template,
    core::uint32 number, BindType::type bind) {
  request::RequestRemoveItem request;
  request.__set_actor_id_(actor);
  request.__set_item_template_(item_template);
  request.__set_number_(number);
  request.__set_bind_((entity::BindRequireType::type)bind);
  if(ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_REMOVE_ITEM, &request,
        sizeof(request)) == 0) {
    return request.remove_number_;
  }
  return 0;
}

}  // namespace chat

}  // namespace server

}  // namespace game

