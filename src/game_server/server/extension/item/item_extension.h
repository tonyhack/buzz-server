//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-21 15:12:13.
// File name: item_extension.h
//
// Description:
// Define class ItemExtension.
//

#ifndef __GAME__SERVER__ITEM__ITEM__EXTENSION__H
#define __GAME__SERVER__ITEM__ITEM__EXTENSION__H

#include "core/base/noncopyable.h"
#include "game_server/server/extension/item/actor_functionality_enable_event_handler.h"
#include "game_server/server/extension/item/actor_logout_event_handler.h"
#include "game_server/server/extension/item/actor_save_event_handler.h"
#include "game_server/server/extension/item/change_vocation_event_handler.h"
#include "game_server/server/extension/item/change_vip_event_handler.h"
#include "game_server/server/extension/item/container_message_handler.h"
#include "game_server/server/extension/item/equip_message_handler.h"
#include "game_server/server/extension/item/extension_load_event_handler.h"
#include "game_server/server/extension/item/actor_daily_clean_event_handler.h"
#include "game_server/server/extension/item/fighting_status_event_handler.h"
#include "game_server/server/extension/item/gem_message_handler.h"
#include "game_server/server/extension/item/item_compose_message_handler.h"
#include "game_server/server/extension/item/mall_message_handler.h"
#include "game_server/server/extension/item/pvp_status_event_handler.h"
#include "game_server/server/extension/item/vip_gifts_message_handler.h"
#include "game_server/server/extension/item/request_add_item.h"
#include "game_server/server/extension/item/request_get_packet_item_number.h"
#include "game_server/server/extension/item/request_remove_item.h"
#include "game_server/server/extension/item/request_get_packet_item_template.h"
#include "game_server/server/extension/item/request_item_handler.h"
#include "game_server/server/extension/item/scene_role_create_event_handler.h"
#include "game_server/server/extension/item/session_message_handler.h"
#include "game_server/server/extension/item/storage_gem_login_response.h"
#include "game_server/server/extension/item/storage_item_actor_login_response.h"
#include "game_server/server/extension/item/storage_item_login_response.h"
#include "game_server/server/extension/item/shop_message_handler.h"
#include "game_server/server/extension/item/treasure_message_handler.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace item {

class ItemExtension : public global::SingletonFactory<ItemExtension> {
 friend class global::SingletonFactory<ItemExtension>;
 public:
  ItemExtension();
  ~ItemExtension();

  bool Initialize();
  void Finalize();

  void Start();
  void Stop();

 private:
  // 数据库回调
  StorageItemActorLoginResponse storage_item_actor_login_;

  StorageItemLoginResponse storage_item_login_;
  StorageGemLoginResponse storage_gem_login_;

  // 事件回调
  ActorLogoutEventHandler actor_logout_event_;
  ActorSaveEventHandler actor_save_event_;
  ActorFunctionalityEnableEventHandler actor_functionality_enable_event_;
  ChangeVocationEventHandler change_vocation_event_;
  ExtensionLoadEventHandler extension_load_event_;
  FightingStatusEventHandler fighting_status_event_;
  PvpStatusEventHandler pvp_status_event_;
  SceneRoleCreateEventHandler scene_role_create_event_;
  ChangeVipEventHandler change_vip_event_;
  ActorDailyCleanEventHandler dailys_event_;

  // 消息回调
  ContainerMessageHandler container_message_handler_;
  EquipMessageHandler equip_message_handler_;
  GemMessageHandler gem_message_handler_;
  ItemComposeMessageHandler item_compose_handler_;
  MallMessageHandler mall_message_handler_;
  ShopMessageHandler shop_message_handler_;
  VipGiftsMessageHandler vip_gifts_message_handler_;
  TreasureMessageHandler treasure_message_handler_;

  // 请求回调
  RequestAddItem add_item_request_;
  RequestRemoveItem req_remove_item_;
  RequestGetPacketItemNumber req_get_packet_item_number_;
  RequestGetPacketItemTemplate req_get_packet_item_template_;
  RequestItemHandler req_item_;

  SessionMessageHandler session_message_handler_;
};

}  // namespace item

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ITEM__ITEM__EXTENSION__H

