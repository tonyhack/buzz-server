//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-08-22 16:34:46.
// File name: team_extension.cc
//
// Description:
// Define classs TeamExtension.
//

#include "session_server/server/extension/team/team_extension.h"

#include "global/logging.h"
#include "global/server_configure.h"
#include "global/server_informations.h"
#include "session_server/server/extension/team/configure.h"
#include "session_server/server/extension/team/team_actor_manager.h"
#include "session_server/server/extension/team/team_actor_pool.h"
#include "session_server/server/extension/team/team_manager.h"
#include "session_server/server/extension/team/team_pool.h"

namespace session {

namespace server {

namespace team {

TeamExtension::TeamExtension() {}
TeamExtension::~TeamExtension() {}

bool TeamExtension::Initialize() {
  if(Configure::GetInstance() == NULL) {
    LOG_ERROR("Configure 不存在");
    return false;
  }

  if(TeamManager::GetInstance() == NULL) {
    LOG_ERROR("组队玩家管理器不存在");
    return false;
  }

  if(TeamPool::GetInstance() == NULL) {
    LOG_ERROR("组队对象池不存在");
    return false;
  }

  if(TeamActorManager::GetInstance() == NULL) {
    LOG_ERROR("组队玩家管理器不存在");
    return false;
  }

  if(TeamActorPool::GetInstance() == NULL) {
    LOG_ERROR("组队玩家对象池不存在");
    return false;
  }

  // 获取游戏服务器信息
  const global::SessionServerInformation *information =
    global::ServerConfigureSingleton::GetInstance().GetSessionServer();
  if(information == NULL) {
    global::LogError("%s:%d (%s) 获取当前服务器配置失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(Configure::GetInstance()->LoadConfigure(
        global::ServerConfigureSingleton::GetInstance().GetTeamFile()) == false) {
    LOG_ERROR("加载组队配置失败");
    return false;
  }

  if(TeamPool::GetInstance()->Initialize(information->user_pool_initial_ / 10,
        information->user_pool_extend_) == false) {
    LOG_ERROR("组队对象池不存在");
    return false;
  }

  if(TeamActorPool::GetInstance()->Initialize(information->user_pool_initial_,
        information->user_pool_extend_) == false) {
    LOG_ERROR("组队玩家对象池不存在");
    return false;
  }

  if(this->game_message_handler_.Initialize() == false) {
    LOG_ERROR("初始化 GameMessageHandler 失败");
    return false;
  }
  if(this->team_message_handler_.Initialize() == false) {
    LOG_ERROR("初始化 TeamMessageHandler 失败");
    return false;
  }

  if(this->actor_jump_scene_handler_.Initialize() == false) {
    LOG_ERROR("初始化 ActorJumpSceneEventHandler 失败");
    return false;
  }
  if(this->actor_levelup_handler_.Initialize() == false) {
    LOG_ERROR("初始化 ActorLevelupEventHandler 失败");
    return false;
  }
  if(this->actor_login_handler_.Initialize() == false) {
    LOG_ERROR("初始化 ActorLoginEventHandler 失败");
    return false;
  }
  if(this->actor_logout_handler_.Initialize() == false) {
    LOG_ERROR("初始化 ActorLogoutEventHandler 失败");
    return false;
  }

  if(this->team_request_handler_.Initialize() == false) {
    LOG_ERROR("初始化 TeamRequestHandler 失败");
    return false;
  }

  return true;
}

void TeamExtension::Finalize() {
  this->team_request_handler_.Finalize();

  this->actor_logout_handler_.Finalize();
  this->actor_login_handler_.Finalize();
  this->actor_levelup_handler_.Finalize();
  this->actor_jump_scene_handler_.Finalize();

  this->team_message_handler_.Finalize();
  this->game_message_handler_.Finalize();

  TeamActorPool::GetInstance()->Finalize();
  TeamPool::GetInstance()->Finalize();

  TeamActorPool::ReleaseInstance();
  TeamPool::ReleaseInstance();
  TeamActorManager::ReleaseInstance();
  TeamManager::ReleaseInstance();
  Configure::ReleaseInstance();
}

void TeamExtension::Start() {}

void TeamExtension::Stop() {}

}  // namespace team

}  // namespace server

}  // namespace session

