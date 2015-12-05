//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-23 12:06:08.
// File name: playing_extension.cc
//
// Description:
// Define class PlayingExtension.
//

#include "game_server/server/extension/playing/playing_extension.h"

#include "game_server/server/game_server.h"
#include "game_server/server/extension/playing/maze_map_manager.h"
#include "game_server/server/extension/playing/playing_actor_manager.h"
#include "game_server/server/extension/playing/playing_actor_pool.h"
#include "game_server/server/extension/playing/playing_builder.h"
#include "game_server/server/extension/playing/playing_destory_checker.h"
#include "game_server/server/extension/playing/playing_manager.h"
#include "game_server/server/extension/playing/playing_page_pool.h"
#include "game_server/server/extension/playing/playing_pool.h"
#include "global/logging.h"
#include "global/server_configure.h"
#include "global/server_informations.h"
#include "global/configure/configure.h"

namespace game {

namespace server {

namespace playing {

PlayingExtension::PlayingExtension() {}
PlayingExtension::~PlayingExtension() {}

bool PlayingExtension::Initialize() {
  // 获取游戏服务器信息
  const global::GameServerInformation *information =
    global::ServerConfigureSingleton::GetInstance().GetGameServers(
        GameServerSingleton::GetInstance().GetSignature().id_);
  if(information == NULL) {
    LOG_ERROR("获取当前服务器配置失败");
    return false;
  }

  const global::configure::PlayingConfigure *playing_configure =
    global::configure::ConfigureSingleton::GetInstance().GetPlayingConfigure();
  if(playing_configure == NULL) {
    LOG_ERROR("获取 PlayingConfigure 失败");
    return false;
  }

  if(MazeMapManager::GetInstance() == NULL) {
    LOG_ERROR("MazeMapManager 不存在");
    return false;
  }

  if(PlayingDestoryChecker::GetInstance() == NULL) {
    LOG_ERROR("PlayingDestoryChecker 不存在");
    return false;
  }

  if(PlayingActorManager::GetInstance() == NULL) {
    LOG_ERROR("PlayingActorManager 不存在");
    return false;
  }

  if (PlayingPagePool::GetInstance() == NULL) {
    LOG_ERROR("PlayingPagePool 不存在");
    return false;
  }

  if(PlayingActorPool::GetInstance() == NULL) {
    LOG_ERROR("PlayingActorPool 不存在");
    return false;
  }

  if(PlayingManager::GetInstance() == NULL) {
    LOG_ERROR("PlayingManager 不存在");
    return false;
  }

  if(PlayingPool::GetInstance() == NULL) {
    LOG_ERROR("PlayingPool 不存在");
    return false;
  }

  if(PlayingBuilder::GetInstance() == NULL) {
    LOG_ERROR("PlayingBuilder 不存在");
    return false;
  }


  if(MazeMapManager::GetInstance()->Initialize() == false) {
    LOG_ERROR("初始化 MazeMapManager 失败");
    return false;
  }

  if(PlayingDestoryChecker::GetInstance()->Initialize() == false) {
    LOG_ERROR("初始化 PlayingDestoryChecker 失败");
    return false;
  }

  if (PlayingPagePool::GetInstance()->Initialize(information->user_pool_initial_ * 10,
        information->user_pool_extend_ * 10) == false) {
    LOG_ERROR("初始化 PlayingPagePool 失败");
    return false;
  }

  if(PlayingActorPool::GetInstance()->Initialize(information->user_pool_initial_,
      information->user_pool_extend_) == false) {
    LOG_ERROR("初始化 PlayingActorPool 失败");
    return false;
  }

  if(PlayingPool::GetInstance()->Initialize(playing_configure->playing_pool_initial_,
      playing_configure->playing_pool_extend_) == false) {
    LOG_ERROR("初始化 PlayingPool 失败");
    return false;
  }

  if(this->actor_load_finish_event_.Initialize() == false) {
    LOG_ERROR("初始化 ActorLoadFinishEventHandler 失败");
    return false;
  }
  if(this->actor_login_event_.Initialize() == false) {
    LOG_ERROR("初始化 ActorLoginEventHandler 失败");
    return false;
  }
  if(this->actor_logout_event_.Initialize() == false) {
    LOG_ERROR("初始化 ActorLogoutEventHandler 失败");
    return false;
  }
  if(this->actor_save_event_.Initialize() == false) {
    LOG_ERROR("初始化 ActorSaveEventHandler 失败");
    return false;
  }
  if(this->actor_functionality_enable_event_.Initialize() == false) {
    LOG_ERROR("初始化 ActorFunctionalityEnableEventHandler 失败");
    return false;
  }
  if(this->actor_daily_clean_event_.Initialize() == false) {
    LOG_ERROR("初始化 ActorDailyCleanEventHandler 失败");
    return false;
  }
  if(this->actor_weekly_clean_event_.Initialize() == false) {
    LOG_ERROR("初始化 ActorWeeklyCleanEventHandler 失败");
    return false;
  }
  if(this->actor_vip_change_event_.Initialize() == false) {
    LOG_ERROR("初始化 ActorVipChangeEventHandler 失败");
    return false;
  }
  if(this->jump_scene_event_.Initialize() == false) {
    LOG_ERROR("初始化 JumpSceneEventHandler 失败");
    return false;
  }
  if(this->login_session_event_.Initialize() == false) {
    LOG_ERROR("初始化 LoginSessionEventHandler 失败");
  }

  if(this->session_message_handler_.Initialize() == false) {
    LOG_ERROR("初始化 SessionMessageHandler 失败");
    return false;
  }
  if(this->session_maze_message_handler_.Initialize() == false) {
    LOG_ERROR("初始化 SessionMazeMessageHandler 失败");
    return false;
  }
  if(this->playing_message_handler_.Initialize() == false) {
    LOG_ERROR("初始化 PlayingMessageHandler 失败");
    return false;
  }
  if(this->maze_message_handler_.Initialize() == false) {
    LOG_ERROR("初始化 MazeMessageHandler 失败");
    return false;
  }

  if(this->storage_playing_actor_login_.Initialize() == false) {
    LOG_ERROR("初始化 StoragePlayingActorLoginResponse 失败");
    return false;
  }

  if(this->request_playing_.Initialize() == false) {
    LOG_ERROR("初始化 RequestDailyPassHandler 失败");
    return false;
  }

  return true;
}

void PlayingExtension::Finalize() {
  this->request_playing_.Finalize();

  this->storage_playing_actor_login_.Finalize();

  this->maze_message_handler_.Finalize();
  this->playing_message_handler_.Finalize();
  this->session_maze_message_handler_.Finalize();
  this->session_message_handler_.Finalize();

  this->login_session_event_.Finalize();
  this->jump_scene_event_.Finalize();
  this->actor_vip_change_event_.Finalize();
  this->actor_weekly_clean_event_.Finalize();
  this->actor_daily_clean_event_.Finalize();
  this->actor_functionality_enable_event_.Finalize();
  this->actor_save_event_.Finalize();
  this->actor_logout_event_.Finalize();
  this->actor_login_event_.Finalize();
  this->actor_load_finish_event_.Finalize();

  PlayingPagePool::GetInstance()->Finalize();
  PlayingActorPool::GetInstance()->Finalize();
  PlayingPool::GetInstance()->Finalize();
  PlayingDestoryChecker::GetInstance()->Finalize();
  MazeMapManager::GetInstance()->Finalize();

  PlayingBuilder::ReleaseInstance();
  PlayingPagePool::ReleaseInstance();
  PlayingActorPool::ReleaseInstance();
  PlayingPool::ReleaseInstance();
  PlayingActorManager::ReleaseInstance();
  PlayingManager::ReleaseInstance();
  PlayingDestoryChecker::ReleaseInstance();
  MazeMapManager::ReleaseInstance();
}

void PlayingExtension::Start() {}

void PlayingExtension::Stop() {}

}  // namespace playing

}  // namespace server

}  // namespace game

