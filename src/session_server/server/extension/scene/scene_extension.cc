//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-26 16:20:28.
// File name: scene_extension.cc
//
// Description:
// Define class SceneExtension.
//

#include "session_server/server/extension/scene/scene_extension.h"

#include "global/logging.h"
#include "session_server/server/extension/scene/scene_manager.h"
#include "session_server/server/extension/scene/scene_pool.h"

namespace session {

namespace server {

namespace scene {

SceneExtension::SceneExtension() {}
SceneExtension::~SceneExtension() {}

bool SceneExtension::Initialize() {
  if(ScenePool::GetInstance() == NULL) {
    LOG_ERROR("获取 ScenePool 失败");
    return false;
  }

  if(SceneManager::GetInstance() == NULL) {
    LOG_ERROR("获取 SceneManager 失败");
    return false;
  }

  if(ScenePool::GetInstance()->Initialize(50, 10) == false) {
    LOG_ERROR("初始化 ScenePool 失败");
    return false;
  }

  if(SceneManager::GetInstance()->Initialize() == false) {
    LOG_ERROR("初始化 SceneManager 失败");
    return false;
  }

  // 场景服务器消息回调
  if(this->game_message_event_.Initialize() == false) {
    LOG_ERROR("初始化 GameMessageHandler 失败");
    return false;
  }
  if (this->scene_message_handler_.Initialize() == false) {
    LOG_ERROR("初始化 SceneMessageHandler 失败");
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
  if(this->server_logout_event_.Initialize() == false) {
    LOG_ERROR("初始化 ServerLogoutEventHandler 失败");
    return false;
  }

  // 请求获取地图玩家回调
  if(this->request_scene_get_actors_.Initialize() == false) {
    LOG_ERROR("初始化 RequestSceneGetActors 失败");
    return false;
  }
  // 请求获取地图场景ID回调
  if(this->request_get_map_scene_id_.Initialize() == false) {
    LOG_ERROR("初始化 RequestGetMapSceneID 失败");
    return false;
  }
  // 请求获取场景所在GameServer
  if(this->request_scene_get_game_server_.Initialize() == false) {
    LOG_ERROR("初始化 RequestSceneGetGameServer 失败");
    return false;
  }
  // 请求跳转地图
  if(this->request_scene_jump_.Initialize() == false) {
    LOG_ERROR("初始化 RequestSceneJump 失败");
    return false;
  }

  return true;
}

void SceneExtension::Finalize() {
  this->request_scene_jump_.Finalize();
  this->request_scene_get_game_server_.Finalize();
  this->request_get_map_scene_id_.Finalize();
  this->request_scene_get_actors_.Finalize();

  this->server_logout_event_.Finalize();
  this->actor_login_event_.Finalize();
  this->actor_logout_event_.Finalize();

  this->scene_message_handler_.Finalize();
  this->game_message_event_.Finalize();

  SceneManager::GetInstance()->Finalize();
  ScenePool::GetInstance()->Finalize();

  SceneManager::ReleaseInstance();
  ScenePool::ReleaseInstance();
}

void SceneExtension::Start() {}

void SceneExtension::Stop() {}

}  // namespace scene

}  // namespace server

}  // namespace session

