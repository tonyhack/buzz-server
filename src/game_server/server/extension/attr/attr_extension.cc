#include "game_server/server/extension/attr/attr_extension.h"

#include <cstddef>

#include "global/logging.h"
#include "global/server_configure.h"
#include "global/server_informations.h"
#include "game_server/server/extension/attr/configure.h"
#include "game_server/server/extension/attr/facade_request.h"
#include "game_server/server/extension/attr/attr_actor_manager.h"
#include "game_server/server/extension/attr/attr_actor_pool.h"

namespace game {

namespace server {

namespace attr {

AttrExtension::AttrExtension() {}
AttrExtension::~AttrExtension() {}

bool AttrExtension::Initialize() {
  if (Configure::GetInstance() == NULL) {
    LOG_ERROR("Init Configure singleton failed.");
    return false;
  }

  if (FacadeRequest::GetInstance() == NULL) {
    LOG_ERROR("Init FacadeRequest singleton failed.");
    return false;
  }

  if (AttrActorPool::GetInstance() == NULL) {
    LOG_ERROR("Init AttrActorPool singleton failed.");
    return false;
  }

  if (AttrActorManager::GetInstance() == NULL) {
    LOG_ERROR("Init AttrActorManager singleton failed.");
    return false;
  }

  // 获取游戏服务器信息
  const global::GameServerInformation *information =
    global::ServerConfigureSingleton::GetInstance().GetGameServers(
        GameServerSingleton::GetInstance().GetSignature().id_);
  if (NULL == information) {
    LOG_ERROR("Get GameServerInformation failed.");
    return false;
  }

  // 加载配置文件
  if (Configure::GetInstance()->LoadConfigure(
          global::ServerConfigureSingleton::GetInstance().GetAttrFile()) == false) {
    LOG_ERROR("Load [attr extension] config file failed.");
    return false;
  }

  // 属性玩家对象池初始化
  if (AttrActorPool::GetInstance()->Initialize(information->user_pool_initial_,
          information->user_pool_extend_) == false) {
      LOG_ERROR("Init AttrActorPool falied.");
      return false;
  }

  // 事件回调初始化
  if (this->extension_load_event_.Initialize() == false) {
    LOG_ERROR("Init ExtensionLoadEventHandler failed.");
    return false;
  }
  if (this->actor_compare_event_.Initialize() == false) {
    LOG_ERROR("Init ActorCompareEventHandler faile.");
    return false;
  }
  if (this->actor_logout_event_.Initialize() == false) {
    LOG_ERROR("Init ActorLogoutEventHandler failed.");
    return false;
  }
  if (this->actor_save_event_.Initialize() == false) {
    LOG_ERROR("Init ActorSaveEventHandler failed.");
    return false;
  }
  if (this->actor_daily_clean_event_.Initialize() == false) {
    LOG_ERROR("Init ActorDailyCleanEventHandler failed.");
    return false;
  }
  if (this->actor_levelup_event_.Initialize() == false) {
    LOG_ERROR("Init ActorLeveupEventHandler failed.");
    return false;
  }
  if (this->actor_functionality_enable_event_.Initialize() == false) {
    LOG_ERROR("Init ActorFunctionalityEnableEventHandler failed.");
    return false;
  }
  if (this->actor_vocation_.Initialize() == false) {
    LOG_ERROR("Init ActorVocationEventHandler failed.");
    return false;
  }
  if (this->scene_role_create_event_.Initialize() == false) {
    LOG_ERROR("Init SceneRoleCreateEventHandler failed.");
    return false;
  }
  if (this->bar_event_.Initialize() == false) {
    LOG_ERROR("Init BarFinishEventHandler failed.");
    return false;
  }
  // 数据库回调初始化
  if (this->storage_attr_actor_login_.Initialize() == false) {
    LOG_ERROR("Init StorageAttrActorLoginResponse failed.");
    return false;
  }
  if (this->storage_spirit_facade_login_.Initialize() == false) {
    LOG_ERROR("Init StorageSpiritFacadeLoginResponse failed.");
    return false;
  }
  // 消息回调初始化
  if (this->noble_message_handler_.Initialize() == false) {
    LOG_ERROR("Init NobleMessageHandler failed.");
    return false;
  }
  if (this->sword_message_handler_.Initialize() == false) {
    LOG_ERROR("Initialize SwordMessageHandler failed.");
    return false;
  }
  if (this->element_message_handler_.Initialize() == false) {
    LOG_ERROR("Initialize ElementMessageHandler failed.");
    return false;
  }
  if (this->spirit_message_handler_.Initialize() == false) {
    LOG_ERROR("Initialize SpiritMessageHandler failed.");
    return false;
  }
  if (this->rune_message_handler_.Initialize() == false) {
    LOG_ERROR("Initialize RuneMessageHandler failed.");
    return false;
  }
  if (this->legendary_weapon_message_handler_.Initialize() == false) {
    LOG_ERROR("Initialize LegendaryWeaponMessageHandler failed.");
    return false;
  }
  // 请求回调初始化
  if (this->noble_request_handler_.Initialize() == false) {
    LOG_ERROR("Initialize noble_request_handler_ failed.");
    return false;
  }
  if (this->spirit_request_handler_.Initialize() == false) {
    LOG_ERROR("Initialize SpiritRequestHandler failed.");
    return false;
  }
  if (this->sword_request_handler_.Initialize() == false) {
    LOG_ERROR("Initialize sword_request_handler_ failed.");
    return false;
  }

  return true;
}

void AttrExtension::Finalize() {
  this->sword_request_handler_.Finalize();
  this->spirit_request_handler_.Finalize();
  this->noble_request_handler_.Finalize();
  this->legendary_weapon_message_handler_.Finalize();
  this->rune_message_handler_.Finalize();
  this->spirit_message_handler_.Finalize();
  this->element_message_handler_.Finalize();
  this->sword_message_handler_.Finalize();
  this->noble_message_handler_.Finalize();
  this->storage_spirit_facade_login_.Finalize();
  this->storage_attr_actor_login_.Finalize();
  this->bar_event_.Finalize();
  this->scene_role_create_event_.Finalize();
  this->actor_vocation_.Finalize();
  this->actor_functionality_enable_event_.Finalize();
  this->actor_levelup_event_.Finalize();
  this->actor_daily_clean_event_.Finalize();
  this->actor_save_event_.Finalize();
  this->actor_logout_event_.Finalize();
  this->actor_compare_event_.Finalize();
  this->extension_load_event_.Finalize();
  AttrActorPool::GetInstance()->Finalize();

  AttrActorManager::ReleaseInstance();
  AttrActorPool::ReleaseInstance();
  FacadeRequest::ReleaseInstance();
  Configure::ReleaseInstance();
}

void AttrExtension::Start() {}
void AttrExtension::Stop() {}

}  // namespace attr

}  // namespace server

}  // namespace game

