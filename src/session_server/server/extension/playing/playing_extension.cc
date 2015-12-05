//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-23 17:03:20.
// File name: playing_extension.cc
//
// Description:
// Define class PlayingExtension.
//

#include "session_server/server/extension/playing/playing_extension.h"

#include "session_server/server/session_server.h"
#include "session_server/server/extension/playing/maze_map_manager.h"
#include "session_server/server/extension/playing/playing_activity_controller.h"
#include "session_server/server/extension/playing/playing_actor_manager.h"
#include "session_server/server/extension/playing/playing_actor_pool.h"
#include "session_server/server/extension/playing/playing_load_balance.h"
#include "session_server/server/extension/playing/playing_manager.h"
#include "session_server/server/extension/playing/playing_page_pool.h"
#include "session_server/server/extension/playing/playing_pool.h"
#include "session_server/server/extension/playing/playing_team_manager.h"
#include "session_server/server/extension/playing/playing_team_pool.h"
#include "global/logging.h"
#include "global/server_configure.h"
#include "global/server_informations.h"
#include "global/configure/configure.h"

namespace session {

namespace server {

namespace playing {

PlayingExtension::PlayingExtension() {}
PlayingExtension::~PlayingExtension() {}

bool PlayingExtension::Initialize() {
  const global::SessionServerInformation *information =
    global::ServerConfigureSingleton::GetInstance().GetSessionServer();
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

  if(PlayingLoadBalance::GetInstance() == NULL) {
    LOG_ERROR("PlayingLoadBalance 不存在");
    return false;
  }

  if(MazeMapManager::GetInstance() == NULL) {
    LOG_ERROR("MazeMapManager 不存在");
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

  if (PlayingTeamManager::GetInstance() == NULL) {
    LOG_ERROR("PlayingTeamManager 不存在");
    return false;
  }

  if (PlayingTeamPool::GetInstance() == NULL) {
    LOG_ERROR("PlayingTeamPool 不存在");
    return false;
  }

  if (PlayingActivityController::GetInstance() == NULL) {
    LOG_ERROR("PlayingActivityController 不存在");
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

  if (PlayingTeamPool::GetInstance()->Initialize(playing_configure->playing_pool_initial_,
        playing_configure->playing_pool_extend_) == false) {
    LOG_ERROR("初始化 PlayingTeamPool 失败");
    return false;
  }

  if (PlayingActivityController::GetInstance()->Initialize() == false) {
    LOG_ERROR("初始化 PlayingActivityController 失败");
    return false;
  }

  if(MazeMapManager::GetInstance()->Initialize() == false) {
    LOG_ERROR("初始化 MazeMapManager 失败");
    return false;
  }

  if (PlayingPagePool::GetInstance()->Initialize(information->user_pool_initial_ * 10,
        information->user_pool_extend_ * 10) == false) {
    LOG_ERROR("初始化 PlayingPagePool 失败");
    return false;
  }

  if (this->actor_change_fight_score_event_.Initialize() == false) {
    LOG_ERROR("初始化 ActorChangeFightScoreEventHandler 失败");
    return false;
  }
  if (this->actor_jump_event_.Initialize() == false) {
    LOG_ERROR("初始化 ActorJumpSceneEventHandler 失败");
    return false;
  }
  if (this->actor_levelup_event_.Initialize() == false) {
    LOG_ERROR("初始化 ActorLevelupEventHandler 失败");
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
  if(this->system_event_.Initialize() == false) {
    LOG_ERROR("初始化 SystemEventHandler 失败");
    return false;
  }

  if(this->game_message_handler_.Initialize() == false) {
    LOG_ERROR("初始化 GameMessageHandler 失败");
    return false;
  }
  if(this->game_maze_message_handler_.Initialize() == false) {
    LOG_ERROR("初始化 GameMazeMessageHandler 失败");
    return false;
  }

  if(this->playing_message_handler_.Initialize() == false) {
    LOG_ERROR("初始化 PlayingMessageHandler 失败");
    return false;
  }

  if(this->playing_request_handler_.Initialize() == false) {
    LOG_ERROR("初始化 PlayingRequestHandler 失败");
    return false;
  }

  return true;
}

void PlayingExtension::Finalize() {
  this->playing_request_handler_.Finalize();
  this->playing_message_handler_.Finalize();
  this->game_maze_message_handler_.Finalize();
  this->game_message_handler_.Finalize();
  this->system_event_.Finalize();
  this->actor_logout_event_.Finalize();
  this->actor_login_event_.Finalize();
  this->actor_levelup_event_.Finalize();
  this->actor_jump_event_.Finalize();
  this->actor_change_fight_score_event_.Finalize();

  PlayingPagePool::GetInstance()->Finalize();
  PlayingActivityController::GetInstance()->Finalize();
  PlayingTeamPool::GetInstance()->Finalize();
  PlayingPool::GetInstance()->Finalize();
  PlayingActorPool::GetInstance()->Finalize();
  MazeMapManager::GetInstance()->Finalize();

  PlayingPagePool::ReleaseInstance();
  PlayingActivityController::ReleaseInstance();
  PlayingTeamPool::ReleaseInstance();
  PlayingTeamManager::ReleaseInstance();
  PlayingPool::ReleaseInstance();
  PlayingManager::ReleaseInstance();
  PlayingActorPool::ReleaseInstance();
  PlayingActorManager::ReleaseInstance();
  MazeMapManager::ReleaseInstance();
  PlayingLoadBalance::ReleaseInstance();
}

void PlayingExtension::Start() {
  PlayingActivityController::GetInstance()->Start();
}

void PlayingExtension::Stop() {
  PlayingActivityController::GetInstance()->Stop();
}

}  // namespace playing

}  // namespace server

}  // namespace session

