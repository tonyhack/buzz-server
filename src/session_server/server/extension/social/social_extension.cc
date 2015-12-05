#include "session_server/server/extension/social/social_extension.h"

#include <cstddef>

#include "global/logging.h"
#include "global/server_configure.h"
#include "global/server_informations.h"
#include "session_server/server/extension/social/facade_request.h"
#include "session_server/server/extension/social/social_actor_manager.h"
#include "session_server/server/extension/social/social_actor_pool.h"
#include "session_server/server/extension/social/homeland_battle_manager.h"

namespace session {

namespace server {

namespace social {

SocialExtension::SocialExtension() {}
SocialExtension::~SocialExtension() {}

bool SocialExtension::Initialize() {
  if (FacadeRequest::GetInstance() == NULL) {
    LOG_ERROR("Init FacadeRequest singleton failed.");
    return false;
  }

  if (SocialActorManager::GetInstance() == NULL) {
    LOG_ERROR("Init SocialActorManager singleton failed.");
    return false;
  }

  if (SocialActorPool::GetInstance() == NULL) {
    LOG_ERROR("Init SocialActorPool singleton failed.");
    return false;
  }

  // 获取服务器信息
  const global::SessionServerInformation *information =
    global::ServerConfigureSingleton::GetInstance().GetSessionServer();
  if (NULL == information) {
    LOG_ERROR("Get SessionServerInformation failed."); 
    return false;
  }

  // 社交玩家对象池初始化
  if (SocialActorPool::GetInstance()->Initialize(information->user_pool_initial_,
          information->user_pool_extend_) == false) {
    LOG_ERROR("Init SocialActorPool failed.");
    return false;
  }

  // 社交玩家管理器初始化
  if (SocialActorManager::GetInstance()->Initialize() == false) {
    LOG_ERROR("Init SocialActorManager failed.");
    return false;
  }

  // 家园战斗管理器初始化
  if (HomelandBattleManager::GetInstance()->Initialize() == false) {
    LOG_ERROR("Init HomelandBattleManager failed.");
    return false;
  }

  // 事件回调初始化
  if (this->actor_login_event_.Initialize() == false) {
    LOG_ERROR("Init ActorLoginEventHandler failed.");
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
  if (this->actor_jump_scene_event_.Initialize() == false) {
    LOG_ERROR("Init ActorJumpSceneEventHandler failed.");
    return false;
  }
  if (this->actor_level_up_event_.Initialize() == false) {
    LOG_ERROR("Init ActorLevelUpEventHandler failed.");
    return false;
  }
  if (this->actor_change_vocation_event_.Initialize() == false) {
    LOG_ERROR("Init ActorChangeVocationEventHandler failed.");
    return false;
  }
  if (this->actor_functionality_enable_event_.Initialize() == false) {
    LOG_ERROR("Init ActorFunctionalityEnableEventHandler failed.");
    return false;
  }
  if (this->actor_daily_clean_event_.Initialize() == false) {
    LOG_ERROR("Init ActorDailyCleanEventHandler failed.");
    return false;
  }

  // 请求回调初始化
  if (this->contacts_request_handler_.Initialize() == false) {
    LOG_ERROR("Init ContactsMessageHandler failed.");
    return false;
  }

  // 消息回调初始化
  if (this->contacts_message_handler_.Initialize() == false) {
    LOG_ERROR("Init ContactsMessageHandler failed.");
    return false;
  }
  if (this->enemy_message_handler_.Initialize() == false) { 
    LOG_ERROR("Init EnemyMessageHandler failed.");
    return false;
  }
  if (this->homeland_message_handler_.Initialize() == false) {
    LOG_ERROR("Init HomelandMessageHandler failed.");
    return false;
  }
  if (this->homeland_battle_message_handler_.Initialize() == false) {
    LOG_ERROR("Init HomelandBattleMessageHandler failed.");
    return false;
  }
  if (this->game_homeland_message_handler_.Initialize() == false) {
    LOG_ERROR("Init GameHomelandMessageHandler failed.");
    return false;
  }

  // 数据库回调初始化
  if (this->storage_social_actor_login_.Initialize() == false) {
    LOG_ERROR("Init StorageSocialActorLoginResponse failed.");
    return false;
  }

  return true;
}

void SocialExtension::Finalize() {
  this->storage_social_actor_login_.Finalize();
  this->contacts_request_handler_.Finalize();
  this->game_homeland_message_handler_.Finalize();
  this->homeland_battle_message_handler_.Finalize();
  this->homeland_message_handler_.Finalize();
  this->enemy_message_handler_.Finalize();
  this->contacts_message_handler_.Finalize();
  this->actor_daily_clean_event_.Finalize();
  this->actor_functionality_enable_event_.Finalize();
  this->actor_change_vocation_event_.Finalize();
  this->actor_level_up_event_.Finalize();
  this->actor_jump_scene_event_.Finalize();
  this->actor_save_event_.Finalize();
  this->actor_logout_event_.Finalize();
  this->actor_login_event_.Finalize();
  HomelandBattleManager::GetInstance()->Finalize();
  SocialActorManager::GetInstance()->Finalize();
  SocialActorPool::GetInstance()->Finalize();

  SocialActorManager::ReleaseInstance();
  SocialActorPool::ReleaseInstance();
  FacadeRequest::ReleaseInstance();
}

void SocialExtension::Start() {}
void SocialExtension::Stop() {}

}  // namespace social

}  // namespace server

}  // namespace session

