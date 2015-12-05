//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-11-22 21:06:48.
// File name: request_get_packet_item_template.cc
//
// Description:
//

#include "game_server/server/extension/item/request_get_packet_item_template.h"

#include <boost/bind.hpp>

#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/item/configure.h"
#include "game_server/server/extension/item/item.h"
#include "game_server/server/extension/item/item_actor.h"
#include "game_server/server/extension/item/item_actor_manager.h"
#include "game_server/server/request/game_item_request_types.h"
#include "game_server/server/request/game_request_types.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace item {

RequestGetPacketItemTemplate::RequestGetPacketItemTemplate() {}
RequestGetPacketItemTemplate::~RequestGetPacketItemTemplate() {}

bool RequestGetPacketItemTemplate::Initialize() {
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_GET_PAKCET_ITEM_TEMPLATE,
      boost::bind(&RequestGetPacketItemTemplate::OnRequest, this, _1, _2));

  return true;
}

void RequestGetPacketItemTemplate::Finalize() {
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_GET_PAKCET_ITEM_TEMPLATE);
}

int RequestGetPacketItemTemplate::OnRequest(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestGetPacketItemTemplate) != size) {
    global::LogError("%s:%d (%s) 请求获取背包道具个数时，参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  request::RequestGetPacketItemTemplate *request = (request::RequestGetPacketItemTemplate *)message;

  // 获取 ItemActor 对象
  ItemActor *actor = ItemActorManager::GetInstance()->Get(request->actor_id_);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 请求获取背包道具个数时，道具玩家(%lu)对象 不存在",
        __FILE__, __LINE__, __FUNCTION__, request->actor_id_);
    return -1;
  }

  // 获取背包容器
  PacketContainer *container = (PacketContainer *)actor->GetContainer(
      entity::ItemContainerType::PACKET);
  if(container == NULL) {
    global::LogError("%s:%d (%s) 请求获取背包道具个数时，道具玩家(%lu) 背包 不存在",
        __FILE__, __LINE__, __FUNCTION__, request->actor_id_);
    return -1;
  }

  // 得到道具模板id
  Item *item = NULL;
  if(request->__isset.location_ == true) {
    item = container->GetItem(request->location_);
  } else {
    if(request->__isset.item_id_ == false) {
      LOG_ERROR("参数错误");
      return -1;
    }
    item = container->GetItemByID(request->item_id_);
  }
  if(item == NULL) {
    LOG_ERROR("道具没找到位置[%d] item_id[%lu]", request->location_, request->item_id_);
    return -1;
  }
  request->__set_item_template_(item->GetTemplateID());
  request->__set_bind_(item->GetAttribute(entity::ItemClientFields::BIND_TYPE) != 0 ? true : false);

  if(request->__isset.item_extra_info_ == true) {
    item->ExportExtraInfo(request->item_extra_info_);
  }

  return 0;
}

}  // namespace item

}  // namespace server

}  // namespace game

