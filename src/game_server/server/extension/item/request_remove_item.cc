//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-20 14:05:41.
// File name: request_remove_item.cc
//
// Description:
// Define class RequestRemoveItem.
//

#include "game_server/server/extension/item/request_remove_item.h"

#include <boost/bind.hpp>

#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/item/configure.h"
#include "game_server/server/extension/item/item.h"
#include "game_server/server/extension/item/item_actor.h"
#include "game_server/server/extension/item/item_builder.h"
#include "game_server/server/extension/item/item_actor_manager.h"
#include "game_server/server/request/game_item_request_types.h"
#include "game_server/server/request/game_request_types.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace item {

RequestRemoveItem::RequestRemoveItem() {}
RequestRemoveItem::~RequestRemoveItem() {}

bool RequestRemoveItem::Initialize() {
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_REMOVE_ITEM,
      boost::bind(&RequestRemoveItem::OnRequest, this, _1, _2));

  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_REMOVE_ITEM_LOCATION_ID,
      boost::bind(&RequestRemoveItem::OnRemoveLocationIDRequest, this, _1, _2));

  return true;
}

void RequestRemoveItem::Finalize() {
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_REMOVE_ITEM);
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_REMOVE_ITEM_LOCATION_ID);
}

int RequestRemoveItem::OnRequest(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestRemoveItem) != size) {
    global::LogError("%s:%d (%s) 请求删除背包道具时，参数错误",
        __FILE__, __LINE__, __FUNCTION__);
  }

  request::RequestRemoveItem *request = (request::RequestRemoveItem *)message;

  // 获取 ItemActor 对象
  ItemActor *actor = ItemActorManager::GetInstance()->Get(request->actor_id_);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 请求删除背包道具时，道具玩家(%lu)对象 不存在",
        __FILE__, __LINE__, __FUNCTION__, request->actor_id_);
    return -1;
  }

  // 获取背包容器
  PacketContainer *container = (PacketContainer *)actor->GetContainer(
      entity::ItemContainerType::PACKET);
  if(container == NULL) {
    global::LogError("%s:%d (%s) 请求删除背包道具时，道具玩家(%lu) 背包 不存在",
        __FILE__, __LINE__, __FUNCTION__, request->actor_id_);
    return -1;
  }

  request->__set_remove_number_(container->RemoveItem(request->item_template_,
      request->number_, request->bind_));

  return 0;
}

int RequestRemoveItem::OnRemoveLocationIDRequest(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestRemoveItemLocationID) != size) {
    global::LogError("%s:%d (%s) 请求删除背包道具时，参数错误",
        __FILE__, __LINE__, __FUNCTION__);
  }

  request::RequestRemoveItemLocationID *request = (request::RequestRemoveItemLocationID *)message;

  // 获取 ItemActor 对象
  ItemActor *actor = ItemActorManager::GetInstance()->Get(request->actor_id_);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 请求删除背包道具时，道具玩家(%lu)对象 不存在",
        __FILE__, __LINE__, __FUNCTION__, request->actor_id_);
    return -1;
  }

  // 获取背包容器
  PacketContainer *container = (PacketContainer *)actor->GetContainer(
      entity::ItemContainerType::PACKET);
  if(container == NULL) {
    global::LogError("%s:%d (%s) 请求删除背包道具时，道具玩家(%lu) 背包 不存在",
        __FILE__, __LINE__, __FUNCTION__, request->actor_id_);
    return -1;
  }
  // 获取对象
  Item *item = NULL;
  if(request->__isset.location_ == true) {
    item = container->GetItem(request->location_);
    if(item == NULL) {
      LOG_ERROR("位置上没有道具[%d]", request->location_);
      return -1;
    }
  } else {
    if(request->__isset.item_id_ == false) {
      LOG_ERROR("删除道具错误");
      return -1;
    }
    item = container->GetItemByID(atol(request->item_id_.c_str()));
    if(item == NULL) {
      LOG_ERROR("没有道具,item_id[%lu]", atol(request->item_id_.c_str()));
      return -1;
    }
  }
  // 扣除
  if(item->GetAttribute(entity::ItemClientFields::STACK_COUNT) < request->number_) {
    LOG_ERROR("指定的位置[%d]上的道具数目[%d]不足", request->location_, 
        item->GetAttribute(entity::ItemClientFields::STACK_COUNT));
    request->__set_result_(request::RemoveResult::ERROR_UNKNOWN);
    return 0;
  } else if(item->GetAttribute(entity::ItemClientFields::STACK_COUNT) > request->number_) {
    item->SetAttribute(entity::ItemClientFields::STACK_COUNT, item->GetAttribute(
          entity::ItemClientFields::STACK_COUNT) - request->number_, true);
    return 0;
  } else {
    if(container->RemoveItem(item, true) == false) {
      LOG_ERROR("删除失败,[%lu]", item->GetID());
      return -1;
    }
    ItemBuilder::GetInstance()->Destory(item);
  }
  request->__set_result_(request::RemoveResult::SUCCESS);
  return 0;
}

}  // namespace item

}  // namespace server

}  // namespace game

