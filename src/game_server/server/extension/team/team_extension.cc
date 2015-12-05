//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-10 14:11:22.
// File name: team_extension.cc
//
// Description:
// Define class TeamExtension.
//

#include "game_server/server/extension/team/team_extension.h"

#include "game_server/server/game_server.h"
#include "game_server/server/extension/team/team_actor_manager.h"
#include "game_server/server/extension/team/team_actor_pool.h"
#include "global/logging.h"
#include "global/server_configure.h"
#include "global/server_informations.h"

namespace game {

namespace server {

namespace team {

TeamExtension::TeamExtension() {}
TeamExtension::~TeamExtension() {}

bool TeamExtension::Initialize() {
  if(TeamActorManager::GetInstance() == NULL) {
    LOG_ERROR("队伍玩家管理器不存在");
    return false;
  }

  if(TeamActorPool::GetInstance() == NULL) {
    LOG_ERROR("队伍玩家对象池不存在");
    return false;
  }

  // 获取游戏服务器信息
  const global::GameServerInformation *information =
    global::ServerConfigureSingleton::GetInstance().GetGameServers(
        GameServerSingleton::GetInstance().GetSignature().id_);
  if(information == NULL) {
    LOG_ERROR("获取当前服务器配置失败");
    return false;
  }

  if(TeamActorPool::GetInstance()->Initialize(information->user_pool_initial_,
        information->user_pool_extend_) == false) {
    LOG_ERROR("初始化 TeamActorPool 失败");
    return false;
  }

  if(this->actor_change_attr_event_.Initialize() == false) {
    LOG_ERROR("初始化 ActorChangeAttributeEventHandler 失败");
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
  if(this->scene_jump_map_event_.Initialize() == false) {
    LOG_ERROR("初始化 SceneJumpMapEventHandler 失败");
    return false;
  }
  if(this->scene_role_create_event_.Initialize() == false) {
    LOG_ERROR("初始化 SceneRoleCreateEventHandler 失败");
    return false;
  }

  if(this->session_message_handler_.Initialize() == false) {
    LOG_ERROR("初始化 SessionMessageHandler 失败");
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

  this->session_message_handler_.Finalize();

  this->scene_role_create_event_.Finalize();
  this->scene_jump_map_event_.Finalize();
  this->actor_logout_event_.Finalize();
  this->actor_login_event_.Finalize();
  this->actor_change_attr_event_.Finalize();

  TeamActorPool::GetInstance()->Finalize();

  TeamActorPool::ReleaseInstance();
  TeamActorManager::ReleaseInstance();
}

void TeamExtension::Start() {}

void TeamExtension::Stop() {}

}  // namespace team

}  // namespace server

}  // namespace game

