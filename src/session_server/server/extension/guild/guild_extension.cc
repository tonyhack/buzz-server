#include "session_server/server/extension/guild/guild_extension.h"

#include <cstddef>

#include "global/logging.h"
#include "global/server_configure.h"
#include "global/server_informations.h"
#include "session_server/server/extension/guild/facade_request.h"
#include "session_server/server/extension/guild/guild_manager.h"
#include "session_server/server/extension/guild/guild_pool.h"
#include "session_server/server/extension/guild/guild_actor_manager.h"
#include "session_server/server/extension/guild/guild_actor_pool.h"

namespace session {

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

  // 获取服务器信息
  const global::SessionServerInformation *information =
    global::ServerConfigureSingleton::GetInstance().GetSessionServer();
  if (NULL == information) {
    LOG_ERROR("Get SessionServerInformation failed.");
    return false;
  }

  // 公会对象池初始化
  if (GuildPool::GetInstance()->Initialize(information->user_pool_initial_ / 10,
          information->user_pool_extend_) == false) {
    LOG_ERROR("Init GuildPool failed.");
    return false;
  }

  // 公会管理器初始化
  if (GuildManager::GetInstance()->Initialize() == false) {
    LOG_ERROR("Init GuildManager failed.");
    return false;
  }

  // 公会玩家对象池初始化
  if (GuildActorPool::GetInstance()->Initialize(information->user_pool_initial_,
          information->user_pool_extend_) == false) {
    LOG_ERROR("Init GuildActorPool failed.");
    return false;
  }

  // 公会玩家管理器初始化
  if (GuildActorManager::GetInstance()->Initialize() == false) {
    LOG_ERROR("Init GuildActorManager failed.");
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
  if (this->actor_jump_login_event_.Initialize() == false) {
    LOG_ERROR("Init ActorJumpLoginEventHandler failed.");
    return false;
  }
  if (this->actor_functionality_enable_event_.Initialize() == false) {
    LOG_ERROR("Init ActorFunctionalityEnableEventHandler failed.");
    return false;
  }
  if (this->actor_levelup_event_.Initialize() == false) {
    LOG_ERROR("Init ActorLevelUpEventHandler failed.");
    return false;
  }
  if (this->actor_change_fight_score_event_.Initialize() == false) {
    LOG_ERROR("Init ActorChangeFightScoreEventHandler failed.");
    return false;
  }
  if (this->actor_weekly_clean_event_.Initialize() == false) {
    LOG_ERROR("Init ActorWeeklyCleanEventHandler failed.");
    return false;
  }
  if (this->guild_daily_clean_event_.Initialize() == false) {
    LOG_ERROR("Init GuildDailyCleanEventHandler failed.");
    return false;
  }
  if (this->guild_weekly_clean_event_.Initialize() == false) {
    LOG_ERROR("Init GuildWeeklyCleanEventHandler failed.");
    return false;
  }

  // 消息回调初始化
  if (this->game_guild_message_handler_.Initialize() == false) {
    LOG_ERROR("Init GameGuildMessageHandler failed.");
    return false;
  }
  if (this->guild_message_handler_.Initialize() == false) {
    LOG_ERROR("Init GuildMessageHandler failed.");
    return false;
  }

  // 消息回调初始化
  if (this->guild_request_handler_.Initialize() == false) {
    LOG_ERROR("Init GuildRequestHandler failed.");
    return false;
  }

  // 数据库回调初始化
  if (this->storage_guild_actor_login_.Initialize() == false) {
    LOG_ERROR("Init StorageGuildActorLoginResponse failed.");
    return false;
  }

  return true;
}

void GuildExtension::Finalize() {
  this->storage_guild_actor_login_.Finalize();
  this->guild_request_handler_.Finalize();
  this->guild_message_handler_.Finalize();
  this->game_guild_message_handler_.Finalize();
  this->guild_weekly_clean_event_.Finalize();
  this->guild_daily_clean_event_.Finalize();
  this->actor_weekly_clean_event_.Finalize();
  this->actor_change_fight_score_event_.Finalize();
  this->actor_levelup_event_.Finalize();
  this->actor_functionality_enable_event_.Finalize();
  this->actor_jump_login_event_.Finalize();
  this->actor_jump_scene_event_.Finalize();
  this->actor_save_event_.Finalize();
  this->actor_logout_event_.Finalize();
  this->actor_login_event_.Finalize();
  GuildActorManager::GetInstance()->Finalize();
  GuildActorPool::GetInstance()->Finalize();
  GuildManager::GetInstance()->Finalize();
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

}  // namespace session

