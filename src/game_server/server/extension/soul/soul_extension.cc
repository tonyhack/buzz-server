//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-17 12:04:26.
// File name: soul_extension.cc
//
// Description:
// Define class SoulExtension.
//

#include "game_server/server/extension/soul/soul_extension.h"

#include "game_server/server/game_server.h"
#include "game_server/server/extension/soul/configure.h"
#include "game_server/server/extension/soul/soul_configure.h"
#include "game_server/server/extension/soul/soul_pool.h"
#include "game_server/server/extension/soul/soul_actor_manager.h"
#include "game_server/server/extension/soul/soul_actor_pool.h"
#include "global/logging.h"
#include "global/server_configure.h"
#include "global/server_informations.h"

namespace game {

namespace server {

namespace soul {

SoulExtension::SoulExtension() {}
SoulExtension::~SoulExtension() {}

bool SoulExtension::Initialize() {
  // 配置文件 
  if(Configure::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) 英灵配置不存在",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 英灵角色管理器
  if(SoulActorManager::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) 英灵玩家管理器不存在",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 英灵玩家对象池
  if(SoulActorPool::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) 英灵玩家对象池不存在",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 英灵对象池
  if(SoulPool::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) 英灵对象池不存在",
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
        global::ServerConfigureSingleton::GetInstance().GetSoulFile()) == false) {
    global::LogError("%s:%d (%s) 加载 英灵配置失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 英灵角色对象池初始化
  if(SoulActorPool::GetInstance()->Initialize(
        information->user_pool_initial_,
        information->user_pool_extend_) == false) {
    global::LogError("%s:%d (%s) 初始化英灵角色对象池失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 英灵对象池初始化
  if(SoulPool::GetInstance()->Initialize(
        Configure::GetInstance()->soul_pool_initial_,
        Configure::GetInstance()->soul_pool_extend_) == false) {
    global::LogError("%s:%d (%s) 初始化英灵对象池失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 事件回调
  if (this->actor_attr_event_.Initialize() == false) {
    LOG_ERROR("初始化 ActorAttrEventHandler 失败");
    return false;
  }
  if(this->actor_daily_clean_event_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 ActorDailyCleanEventHandler 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->actor_functionality_enable_event_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 ActorFunctionalityEnableEventHandler 失败",
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
  if (this->attr_event_.Initialize() == false) {
    LOG_ERROR("初始化 AttrEventHandler 失败");
    return false;
  }
  if(this->extension_load_event_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 ExtensionLoadEventHandler 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->fighting_status_event_.Initialize() == false) {
    LOG_ERROR("初始化 FightingStatusEventHandler 失败");
    return false;
  }
  if (this->role_killed_event_.Initialize() == false) {
    LOG_ERROR("初始化 RoleKilledEventHandler 失败");
    return false;
  }

  // 请求回调
  if(this->req_soul_operate_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 RequestSoulOperate 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 存储过程回调
  if(this->storage_soul_login_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 StorageSoulLoginResponse 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->storage_soul_actor_login_.Initialize() == false) {
    LOG_ERROR("初始化 StorageSoulSettingLoginResponse 失败");
    return false;
  }

  // 消息回调
  if(this->soul_message_handler_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 SoulMessageHandler 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->soul_altar_message_handler_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 SoulAltarMessageHandler 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  return true;
}

void SoulExtension::Finalize() {
  this->soul_altar_message_handler_.Finalize();
  this->soul_message_handler_.Finalize();

  this->req_soul_operate_.Finalize();

  this->storage_soul_actor_login_.Finalize();
  this->storage_soul_login_.Finalize();

  this->role_killed_event_.Finalize();
  this->fighting_status_event_.Finalize();
  this->extension_load_event_.Finalize();
  this->attr_event_.Finalize();
  this->actor_save_event_.Finalize();
  this->actor_logout_event_.Finalize();
  this->actor_functionality_enable_event_.Finalize();
  this->actor_daily_clean_event_.Finalize();
  this->actor_attr_event_.Finalize();

  SoulPool::GetInstance()->Finalize();
  SoulActorPool::GetInstance()->Finalize();

  SoulPool::ReleaseInstance();
  SoulActorPool::ReleaseInstance();
  Configure::ReleaseInstance();
}

void SoulExtension::Start() {}

void SoulExtension::Stop() {}

}  // namespace soul

}  // namespace server

}  // namespace game

