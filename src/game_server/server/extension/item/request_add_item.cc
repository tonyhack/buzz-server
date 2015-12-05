//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-22 19:16:25.
// File name: request_add_item.cc
//
// Description:
//

#include "game_server/server/extension/item/request_add_item.h"

#include <string>
#include <boost/bind.hpp>

#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/item/configure.h"
#include "game_server/server/extension/item/item.h"
#include "game_server/server/extension/item/item_actor.h"
#include "game_server/server/extension/item/item_actor_manager.h"
#include "game_server/server/extension/item/item_builder.h"
#include "game_server/server/request/game_item_request_types.h"
#include "game_server/server/request/game_request_types.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace item {

RequestAddItem::RequestAddItem() {}
RequestAddItem::~RequestAddItem() {}

bool RequestAddItem::Initialize() {
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_ADD_ITEM,
      boost::bind(&RequestAddItem::OnRequest, this, _1, _2));

  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_ADD_ITEMS,
      boost::bind(&RequestAddItem::OnRequestAddItems, this, _1, _2));

  return true;
}

void RequestAddItem::Finalize() {
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_ADD_ITEM);

  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_ADD_ITEMS);
}

int RequestAddItem::OnRequest(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestAddItem) != size) {
    global::LogError("%s:%d (%s) 请求增加道具时，参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  request::RequestAddItem *request = (request::RequestAddItem *)message;

  if(request->number_ <= 0) {
    LOG_ERROR("增加道具时，数量为0");
    return -1;
  }

  // 获取 ItemActor 对象
  ItemActor *actor = ItemActorManager::GetInstance()->Get(request->actor_id_);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 请求增加道具时，道具玩家(%lu)对象 不存在",
        __FILE__, __LINE__, __FUNCTION__, request->actor_id_);
    return -1;
  }

  PacketContainer *container = (PacketContainer *)actor->GetContainer(
      entity::ItemContainerType::PACKET);
  if(container == NULL) {
    global::LogError("%s:%d (%s) 请求增加道具时，道具玩家(%lu) 背包 不存在",
        __FILE__, __LINE__, __FUNCTION__, request->actor_id_);
    return -1;
  }

  if(request->__isset.location_ == true) {
    // 添加到指定位置
    if(container->AddItemsByLocation(request->location_, request->item_template_,
          request->number_, true, request->bind_, request->intensify_level_) == false) {
      LOG_ERROR("添加道具失败");
      request->__set_result_(request::AddItemResult::ERROR_UNKNOWN);
      return 0;
    }
  } else {
    if(container->CheckAddItem(request->item_template_, request->number_, 
          request->bind_ == 0 ? false : true) == false) {
      request->__set_result_(request::AddItemResult::ERROR_UNKNOWN);
      return 0;
    }
  }

  // 是否需要添加道具
  if(request->__isset.simulate_ && request->simulate_) {
    request->__set_result_(request::AddItemResult::SUCCESS);
    return 0;
  }

  // 道具额外信息
  const std::string *item_extra_info = NULL;
  if (request->__isset.item_extra_info_) {
    item_extra_info = &request->item_extra_info_;
  }

  // 加入道具，指定位置或者不指定
  if(request->__isset.location_ != true) {
    if(container->AddItems(request->item_template_, request->number_,
          request->bind_, request->intensify_level_, item_extra_info) == false) {
      request->__set_result_(request::AddItemResult::ERROR_UNKNOWN);
      return 0;
    }
  } else {
    // 添加到指定位置
    if(container->AddItemsByLocation(request->location_, request->item_template_,
          request->number_, false, request->bind_, request->intensify_level_, item_extra_info) == false) {
      request->__set_result_(request::AddItemResult::ERROR_UNKNOWN);
      return 0;
    }
  }
  request->__set_result_(request::AddItemResult::SUCCESS);
  return 0;
}

int RequestAddItem::OnRequestAddItems(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestAddItems) != size) {
    global::LogError("%s:%d (%s) 请求增加道具时，参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  request::RequestAddItems *request = (request::RequestAddItems *)message;

  // 获取 ItemActor 对象
  ItemActor *actor = ItemActorManager::GetInstance()->Get(request->actor_id_);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 请求增加道具时，道具玩家(%lu)对象 不存在",
        __FILE__, __LINE__, __FUNCTION__, request->actor_id_);
    return -1;
  }

  PacketContainer *container = (PacketContainer *)actor->GetContainer(
      entity::ItemContainerType::PACKET);
  if(container == NULL) {
    global::LogError("%s:%d (%s) 请求增加道具时，道具玩家(%lu) 背包 不存在",
        __FILE__, __LINE__, __FUNCTION__, request->actor_id_);
    return -1;
  }

  if(container->CheckAddItems(request->items_) == false) {
    return -1;
  }
  // 只检查, 不添加道具
  if(request->__isset.simulate_ && request->simulate_) {
    request->__set_result_(request::AddItemResult::SUCCESS);
    return 0;
  }

  // 增加到背包
  for(size_t pos = 0; pos < request->items_.size(); ++pos) {
    if(container->AddItems(request->items_[pos].item_template_,
          request->items_[pos].number_, request->items_[pos].bind_, request->items_[pos].intensify_level_) == false) {
      request->__set_result_(request::AddItemResult::ERROR_UNKNOWN);
      return 0;
    }
  }

  request->__set_result_(request::AddItemResult::SUCCESS);

  return 0;
}

}  // namespace item

}  // namespace server

}  // namespace game

