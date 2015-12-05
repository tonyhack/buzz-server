//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-20 12:33:18.
// File name: request_get_packet_item_number.cc
//
// Description:
// Define class RequestGetPacketItemNumber.
//

#include "game_server/server/extension/item/request_get_packet_item_number.h"

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

RequestGetPacketItemNumber::RequestGetPacketItemNumber() {}
RequestGetPacketItemNumber::~RequestGetPacketItemNumber() {}

bool RequestGetPacketItemNumber::Initialize() {
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_GET_PAKCET_ITEM_NUMBER,
      boost::bind(&RequestGetPacketItemNumber::OnRequest, this, _1, _2));

  return true;
}

void RequestGetPacketItemNumber::Finalize() {
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_GET_PAKCET_ITEM_NUMBER);
}

int RequestGetPacketItemNumber::OnRequest(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestGetPacketItemNumber) != size) {
    global::LogError("%s:%d (%s) 请求获取背包道具个数时，参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  request::RequestGetPacketItemNumber *request = (request::RequestGetPacketItemNumber *)message;

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

  // 得到道具个数
  request->__set_number_(container->GetItemNumber(request->item_template_,
        request->bind_));

  return 0;
}

}  // namespace item

}  // namespace server

}  // namespace game

