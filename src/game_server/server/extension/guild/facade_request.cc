#include "game_server/server/extension/guild/facade_request.h"

#include "game_server/server/extension_manager.h"
#include "game_server/server/request/game_request_types.h"
#include "game_server/server/request/game_buff_request_types.h"
#include "game_server/server/request/game_item_request_types.h"

namespace game {

namespace server {

namespace guild {

bool FacadeRequest::CheckPacketItemExist(core::uint64 actor,
    core::uint32 item_template, core::uint32 number, entity::BindRequireType::type bind) {
  request::RequestGetPacketItemNumber request;
  request.__set_actor_id_(actor);
  request.__set_item_template_(item_template);
  request.__set_bind_(bind);
  if(ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_GET_PAKCET_ITEM_NUMBER, &request,
        sizeof(request)) == 0 && (core::uint32)request.number_ >= number) {
    return true;
  }
  return false;
}

core::uint32 FacadeRequest::RemovePacketItem(core::uint64 actor, core::uint32 item_template,
    core::uint32 number, entity::BindRequireType::type bind) {
  request::RequestRemoveItem request;
  request.__set_actor_id_(actor);
  request.__set_item_template_(item_template);
  request.__set_number_(number);
  request.__set_bind_(bind);
  if(ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_REMOVE_ITEM, &request,
        sizeof(request)) == 0) {
    return request.remove_number_;
  }
  return 0;
}

bool FacadeRequest::CheckAddPacketItem(core::uint64 actor, core::uint32 item_template,
    core::uint32 number, entity::BindRequireType::type bind) {
  request::RequestAddItem request;
  request.__set_actor_id_(actor);
  request.__set_item_template_(item_template);
  request.__set_number_(number);
  request.__set_bind_(bind);
  request.__set_simulate_(true);
  if(ExtensionManager::GetInstance()->Request(
         request::RequestType::REQUEST_ADD_ITEM, &request,
         sizeof(request)) == 0 && request.result_ == request::AddItemResult::SUCCESS) {
    return true;
  }
  return false;
}

bool FacadeRequest::CheckAddPacketItems(core::uint64 actor,
    const std::vector<entity::AddItemFields> &items) {
  request::RequestAddItems request;
  request.__set_actor_id_(actor);
  request.__set_items_(items);
  request.__set_simulate_(true);
  if(ExtensionManager::GetInstance()->Request(
         request::RequestType::REQUEST_ADD_ITEMS, &request,
         sizeof(request)) == 0 && request.result_ == request::AddItemResult::SUCCESS) {
    return true;
  }
  return false;
}

bool FacadeRequest::AddPacketItem(core::uint64 actor, core::uint32 item_template,
    core::uint32 number, entity::BindRequireType::type bind) {
  request::RequestAddItem request;
  request.__set_actor_id_(actor);
  request.__set_item_template_(item_template);
  request.__set_number_(number);
  request.__set_bind_(bind);
  if(ExtensionManager::GetInstance()->Request(
         request::RequestType::REQUEST_ADD_ITEM, &request,
         sizeof(request)) == 0 && request.result_ == request::AddItemResult::SUCCESS) {
    return true;
  }
  return false;
}

bool FacadeRequest::AddPacketItems(core::uint64 actor,
    const std::vector<entity::AddItemFields> &items) {
  request::RequestAddItems request;
  request.__set_actor_id_(actor);
  request.__set_items_(items);
  if(ExtensionManager::GetInstance()->Request(
         request::RequestType::REQUEST_ADD_ITEMS, &request,
         sizeof(request)) == 0 && request.result_ == request::AddItemResult::SUCCESS) {
    return true;
  }
  return false;
}

bool FacadeRequest::AddBuff(core::uint64 actor, core::uint32 buff) {
  request::RequestBuffStart request;
  request.__set_src_entity_type_(entity::EntityType::TYPE_ACTOR);
  request.__set_src_entity_id_(actor);
  request.__set_dest_entity_type_(entity::EntityType::TYPE_ACTOR);
  request.__set_dest_entity_id_(actor);
  request.__set_buff_id_(buff);
  if(ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_BUFF_START, &request,
        sizeof(request)) == 0 && request.succeed_ == true) {
    return true;
  }
  return false;
}

bool FacadeRequest::RemoveBuff(core::uint64 actor, core::uint32 buff) {
  request::RequestBuffEnd request;
  request.__set_dest_entity_type_(entity::EntityType::TYPE_ACTOR);
  request.__set_dest_entity_id_(actor);
  request.__set_buff_id_(buff);
  if(ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_BUFF_END, &request,
        sizeof(request)) == 0 && request.succeed_ == true) {
    return true;
  }
  return false;
}

}  // namespace guild

}  // namespace server

}  // namespace game

