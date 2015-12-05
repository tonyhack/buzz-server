#include "game_server/server/extension/guild/guild_extension.h"

#include <cstddef>

#include "global/logging.h"
#include "global/server_configure.h"
#include "global/server_informations.h"
#include "global/configure/configure.h"
#include "game_server/server/extension/guild/facade_request.h"
#include "game_server/server/extension/guild/guild_manager.h"
#include "game_server/server/extension/guild/guild_pool.h"
#include "game_server/server/extension/guild/guild_actor_manager.h"
#include "game_server/server/extension/guild/guild_actor_pool.h"

namespace game {

namespace server {

namespace guild {

GuildExtension::GuildExtension() {}
GuildExtension::~GuildExtension() {}

bool GuildExtension::Initialize() {
  if (FacadeRequest::GetInstance() == NULL) {
    LOG_ERROR("Init FacadeRequest singleton failed.");
    return false;
  }

  if (GuildPool::GetInstance() == NULL) {
    LOG_ERROR("Init GuildPool singleton failed.");
    return false;
  }

  if (GuildManager::GetInstance() == NULL) {
    LOG_ERROR("Init GuildManager singleton failed.");
    return false;
  }

  if (GuildActorPool::GetInstance() == NULL) {
    LOG_ERROR("Init GuildActorPool singleton failed.");
    return false;
  }

  if (GuildActorManager::GetInstance() == NULL) {
    LOG_ERROR("Init GuildActorManager singleton failed.");
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

  // 公会对象池初始化
  if (GuildPool::GetInstance()->Initialize(information->user_pool_initial_ / 10, 
          information->user_pool_extend_) == false) {
    LOG_ERROR("Init GuildPool failed.");
    return false;
  }

  // 公会玩家对象池初始化
  if (GuildActorPool::GetInstance()->Initialize(information->user_pool_initial_,
          information->user_pool_extend_) == false) {
    LOG_ERROR("Init GuildActorPool failed.");
    return false;
  }

  // 事件回调初始化
  if (this->login_session_event_.Initialize() == false) {
    LOG_ERROR("Init LoginSessionEventHandler failed.");
    return false;
  }
  if (this->extension_load_event_.Initialize() == false) {
    LOG_ERROR("Init ExtensionLoadEventHandler failed.");
    return false;
  }
  if (this->actor_logout_event_.Initialize() == false) {
    LOG_ERROR("Init ActorLogoutEventHandler failed.");
    return false;
  }
  if (this->scene_role_create_event_.Initialize() == false) {
    LOG_ERROR("Init SceneRoleCreateEventHandler failed.");
    return false;
  }

  // 消息回调初始化
  if (this->session_guild_message_handler_.Initialize() == false) {
    LOG_ERROR("Init SessionGuildMessageHandler failed.");
    return false;
  }
  if (this->guild_message_handler_.Initialize() == false) {
    LOG_ERROR("Init GuildMessageHandler failed.");
    return false;
  }

  if (this->guild_request_handler_.Initialize() == false) {
    LOG_ERROR("Init GuildRequestHandler failed.");
    return false;
  }

  return true;
}

void GuildExtension::Finalize() {
  this->guild_request_handler_.Finalize();

  this->guild_message_handler_.Finalize();
  this->session_guild_message_handler_.Finalize();
  this->scene_role_create_event_.Finalize();
  this->actor_logout_event_.Finalize();
  this->extension_load_event_.Finalize();
  this->login_session_event_.Finalize();
  GuildActorPool::GetInstance()->Finalize();
  GuildPool::GetInstance()->Finalize();

  GuildActorManager::ReleaseInstance();
  GuildActorPool::ReleaseInstance();
  GuildManager::ReleaseInstance();
  GuildPool::ReleaseInstance();
  FacadeRequest::ReleaseInstance();
}

void GuildExtension::Start() {}
void GuildExtension::Stop() {}

}  // namespace guild

}  // namespace server

}  // namespace game

