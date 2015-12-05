// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-21 15:56:11.
// File name: item_extension.cc
//
// Description:
// Define class ItemExtension.
//

#include "game_server/server/extension/item/item_extension.h"

#include "game_server/server/game_server.h"
#include "game_server/server/extension/item/configure.h"
#include "game_server/server/extension/item/item_actor_manager.h"
#include "game_server/server/extension/item/item_actor_pool.h"
#include "game_server/server/extension/item/item_builder.h"
#include "game_server/server/extension/item/item_manager.h"
#include "game_server/server/extension/item/item_pool.h"
#include "global/logging.h"
#include "global/server_configure.h"
#include "global/server_informations.h"

namespace game {

namespace server {

namespace item {

ItemExtension::ItemExtension() {}
ItemExtension::~ItemExtension() {}

bool ItemExtension::Initialize() {
  // 配置文件
  if(Configure::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) 道具配置不存在",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 道具玩家对象管理器
  if(ItemActorManager::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) 道具玩家管理器不存在",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 道具玩家对象池
  if(ItemActorPool::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) 道具玩家对象池不存在",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 道具管理器
  if(ItemManager::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) 道具管理器不存在",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 道具对象池
  if(ItemPool::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) 道具对象池不存在",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 道具创建器
  if(ItemBuilder::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) ItemBuilder 不存在",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 获取游戏服务器信息
  const global::GameServerInformation *information =
    global::ServerConfigureSingleton::GetInstance().GetGameServers(
        GameServerSingleton::GetInstance().GetSignature().id_);
  if(information == NULL) {
    global::LogError("%s:%d (%s) 获取当前服务器配置失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 加载配置文件
  if(Configure::GetInstance()->LoadConfigure(
        global::ServerConfigureSingleton::GetInstance().GetConfigueDir() +
        global::ServerConfigureSingleton::GetInstance().GetItemFile()) == false) {
    global::LogError("%s:%d (%s) 加载 item 配置失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 初始化道具玩家对象池
  if(ItemActorPool::GetInstance()->Initialize(information->user_pool_initial_,
        information->user_pool_extend_) == false) {
    global::LogError("%s:%d (%s) 初始化道具玩家对象池失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 初始化道具对象池
  if(ItemPool::GetInstance()->Initialize(Configure::GetInstance()->item_initial_number_,
      Configure::GetInstance()->item_extend_number_) == false) {
    global::LogError("%s:%d (%s) 初始化道具对象池失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 数据库回调
  if(storage_item_actor_login_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 StorageItemActorLoginResponse 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(storage_item_login_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 StorageItemLoginResponse 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(storage_gem_login_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 StorageGemLoginResponse 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 事件回调
  if(this->extension_load_event_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 ExtensionLoadEventHandler 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->actor_logout_event_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 ActorLogoutEventHandler 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->actor_save_event_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 ActorSaveEventHandler 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->actor_functionality_enable_event_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 ActorFunctionalityEnableEventHandler 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->change_vocation_event_.Initialize() == false) {
    LOG_ERROR("初始化 ChangeVocationEventHandler 失败");
    return false;
  }
  if(this->scene_role_create_event_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 SceneRoleCreateEventHandler 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->fighting_status_event_.Initialize() == false) {
    LOG_ERROR("初始化 FightingStatusEventHandler 失败");
    return false;
  }
  if(this->pvp_status_event_.Initialize() == false) {
    LOG_ERROR("初始化 PvpStatusEventHandler 失败");
    return false;
  }
  if(this->change_vip_event_.Initialize() == false) {
    LOG_ERROR("初始化 ChangeVipEventHandler 失败");
    return false;
  }
  if(this->dailys_event_.Initialize() == false) {
    LOG_ERROR("初始化 ActorDailyCleanEventHandler 失败");
    return false;
  }

  // 消息回调
  if(this->container_message_handler_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 ContainerMessageHandler 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->equip_message_handler_.Initialize() == false) {
    LOG_ERROR("初始化 EquipMessageHandler 失败");
    return false;
  }
  if(this->gem_message_handler_.Initialize() == false) {
    LOG_ERROR("初始化 GemMessageHandler 失败");
    return false;
  }
  if(this->item_compose_handler_.Initialize() == false) {
    LOG_ERROR("初始化 ItemComposeMessageHandler 失败");
    return false;
  }
  if(this->mall_message_handler_.Initialize() == false) {
    LOG_ERROR("初始化 MallMessageHandler 失败");
    return false;
  }
  if(this->shop_message_handler_.Initialize() == false) {
    LOG_ERROR("初始化 ShopMessageHandler 失败");
    return false;
  }
  if(this->vip_gifts_message_handler_.Initialize() == false) {
    LOG_ERROR("初始化 ActorDailyCleanEventHandler 失败");
    return false;
  }
  if(this->treasure_message_handler_.Initialize() == false) {
    LOG_ERROR("初始化 TreasureMessageHandler 失败");
    return false;
  }

  // 请求回调
  if(this->add_item_request_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 RequestAddItem 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->req_remove_item_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 RequestRemoveItem 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->req_get_packet_item_number_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 RequestGetPacketItemNumber 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->req_get_packet_item_template_.Initialize() == false) {
    LOG_ERROR("初始化 RequestGetPacketItemTemplate 失败");
    return false;
  }
  if(this->req_item_.Initialize() == false) {
    LOG_ERROR("初始化 RequestItemHandler 失败");
    return false;
  }

  // Session 消息回调
  if(this->session_message_handler_.Initialize() == false) {
    LOG_ERROR("初始化 SessionMessageHandler 失败");
    return false;
  }

  return true;
}

void ItemExtension::Finalize() {
  this->session_message_handler_.Finalize();

  this->req_get_packet_item_number_.Finalize();
  this->req_remove_item_.Finalize();
  this->add_item_request_.Finalize();
  this->req_get_packet_item_template_.Finalize();
  this->req_item_.Finalize();

  this->item_compose_handler_.Finalize();
  this->gem_message_handler_.Finalize();
  this->equip_message_handler_.Finalize();
  this->container_message_handler_.Finalize();
  this->mall_message_handler_.Finalize();
  this->vip_gifts_message_handler_.Finalize();

  this->pvp_status_event_.Finalize();
  this->fighting_status_event_.Finalize();
  this->scene_role_create_event_.Finalize();
  this->change_vocation_event_.Finalize();
  this->actor_functionality_enable_event_.Finalize();
  this->actor_save_event_.Finalize();
  this->actor_logout_event_.Finalize();
  this->extension_load_event_.Finalize();
  this->change_vip_event_.Finalize();
  this->dailys_event_.Finalize();
  this->treasure_message_handler_.Finalize();

  this->storage_gem_login_.Finalize();
  this->storage_item_login_.Finalize();
  this->storage_item_actor_login_.Finalize();

  ItemPool::GetInstance()->Finalize();
  ItemActorPool::GetInstance()->Finalize();

  ItemBuilder::ReleaseInstance();

  ItemPool::ReleaseInstance();
  ItemManager::ReleaseInstance();
  ItemActorPool::ReleaseInstance();
  ItemActorManager::ReleaseInstance();
  Configure::ReleaseInstance();
}

void ItemExtension::Start() {}

void ItemExtension::Stop() {}

}  // namespace item

}  // namespace server

}  // namespace game

