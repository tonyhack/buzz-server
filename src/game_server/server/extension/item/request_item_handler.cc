
#include "game_server/server/extension/item/request_item_handler.h"

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

RequestItemHandler::RequestItemHandler() {}
RequestItemHandler::~RequestItemHandler() {}

bool RequestItemHandler::Initialize() {
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_ITEM_WEARED_EQUIP_INFO,
      boost::bind(&RequestItemHandler::OnRequestEquipWeardInfo, this, _1, _2));

  return true;
}

void RequestItemHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_ITEM_WEARED_EQUIP_INFO);
}

int RequestItemHandler::OnRequestEquipWeardInfo(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestItemWearedEquipInfo) != size) {
    global::LogError("%s:%d (%s) 请求装备栏信息，错误",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  request::RequestItemWearedEquipInfo *request = (request::RequestItemWearedEquipInfo *)message;

  // 获取 ItemActor 对象
  ItemActor *actor = ItemActorManager::GetInstance()->Get(request->actor_id_);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 请求增加道具时，道具玩家(%lu)对象 不存在",
        __FILE__, __LINE__, __FUNCTION__, request->actor_id_);
    return -1;
  }

  ItemContainer *con = actor->GetContainer(entity::ItemContainerType::EQUIP,
      actor->GetActorVocation());
  if(con == NULL) {
    LOG_ERROR("获取玩家装备栏(%d)", actor->GetActorVocation());
    return -1;
  }

  EquipmentContainer *equip_con = (EquipmentContainer *)con;

  if(request->type_ == entity::AchieveWearEquipType::QUALITY) {
    request->num_ = equip_con->OverQualityEquip(request->param1_);
  }
  if(request->type_ == entity::AchieveWearEquipType::STRENGTH_LEVEL) {
    request->num_ = equip_con->OverStrengthLevelEquip(request->param1_);
  }
  return 0;
}

}  // namespace item

}  // namespace server

}  // namespace game

