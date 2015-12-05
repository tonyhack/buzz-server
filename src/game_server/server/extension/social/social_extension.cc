#include "game_server/server/extension/social/social_extension.h"

#include <cstddef>

#include "global/logging.h"
#include "global/server_configure.h"
#include "global/server_informations.h"
#include "game_server/server/extension/social/facade_request.h"
#include "game_server/server/extension/social/social_actor_manager.h"
#include "game_server/server/extension/social/social_actor_pool.h"

namespace game {

namespace server {

namespace social {

SocialExtension::SocialExtension() {}
SocialExtension::~SocialExtension() {}

bool SocialExtension::Initialize() {
  if (FacadeRequest::GetInstance() == NULL) {
    LOG_ERROR("Init FacadeRequest singleton failed.");
    return false;
  }

  if (SocialActorPool::GetInstance() == NULL) {
    LOG_ERROR("Init SocialActorPool singleton failed.");
    return false;
  }

  if (SocialActorManager::GetInstance() == NULL) {
    LOG_ERROR("Init SocialActorManager singleton failed.");
    return false;
  }

  // 获取游戏服务器信息
  const global::GameServerInformation *information =
    global::ServerConfigureSingleton::GetInstance().GetGameServers(
        GameServerSingleton::GetInstance().GetSignature().id_);
  if (information == NULL) {
    LOG_ERROR("Get GameServerInformation failed.");
    return false;
  }

  // 社交玩家对象池初始化
  if (SocialActorPool::GetInstance()->Initialize(information->user_pool_initial_,
          information->user_pool_extend_) == false) {
    LOG_ERROR("Init SocialActorPool failed.");
    return false;
  }

  // 事件回调初始化
  if (this->extension_load_event_.Initialize() == false) {
    LOG_ERROR("Init ExtensionLoadEventHandler failed.");
    return false;
  }
  if (this->actor_logout_event_.Initialize() == false) {
    LOG_ERROR("Init ActorLogoutEventHandler failed.");
    return false;
  }
  if (this->soul_add_event_.Initialize() == false) {
    LOG_ERROR("Init SoulAddEventHandler failed.");
    return false;
  }
  if (this->soul_upgrade_event_.Initialize() == false) {
    LOG_ERROR("Init SoulUpgradeEventHandler failed.");
    return false;
  }
  
  // 消息回调初始化
  if (this->session_homeland_message_handler_.Initialize() == false) {
    LOG_ERROR("Init SessionHomelandMessageHandler failed.");
    return false;
  }

  return true;
}

void SocialExtension::Finalize() {
  this->session_homeland_message_handler_.Finalize();
  this->soul_upgrade_event_.Finalize();
  this->soul_add_event_.Finalize();
  this->actor_logout_event_.Finalize();
  this->extension_load_event_.Finalize();
  SocialActorPool::GetInstance()->Finalize();

  SocialActorManager::ReleaseInstance();
  SocialActorPool::ReleaseInstance();
  FacadeRequest::ReleaseInstance();
}

void SocialExtension::Start() {}
void SocialExtension::Stop() {}

}  // namespace social

}  // namespace server

}  // namespace game

