//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-02 16:20:49.
// File name: python_extension.cc
//
// Description:
// Define class PythonExtension.
//

#include "game_server/server/extension/python/python_extension.h"

#include "game_server/server/extension/python/python_actor_pool.h"
#include "game_server/server/extension/python/python_actor_manager.h"
#include "game_server/server/extension/python/python_engine.h"
#include "game_server/server/extension/python/python_npc_pool.h"
#include "game_server/server/extension/python/python_npc_manager.h"
#include "game_server/server/game_server.h"
#include "global/logging.h"
#include "global/server_configure.h"

namespace game {

namespace server {

namespace python {

PythonExtension::PythonExtension() {}
PythonExtension::~PythonExtension() {}

bool PythonExtension::Initialize() {
  // 玩家管理器
  if(PythonActorManager::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) Python 玩家管理器不存在",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 玩家对象池
  if(PythonActorPool::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) Python 玩家对象池不存在",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // Npc管理器
  if(PythonNpcManager::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) PythonNpc 管理器不存在",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // Npc对象池
  if(PythonNpcPool::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) PythonNpc 对象池不存在",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // Python引擎
  if(PythonEngine::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) Python引擎不存在",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // Python引擎初始化
  if(PythonEngine::GetInstance()->Initialize(
        global::ServerConfigureSingleton::GetInstance().GetConfigueDir() +
        global::ServerConfigureSingleton::GetInstance().GetPythonFile()) == false) {
    global::LogError("%s:%d (%s) Python引擎初始化失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 获取游戏服务器信息
  const global::GameServerInformation *information =
    global::ServerConfigureSingleton::GetInstance().GetGameServers(
        GameServerSingleton::GetInstance().GetSignature().id_);
  if(information == NULL) {
    global::LogError("%s:%d (%s) 获取当前服务器配置失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 玩家对象池初始化
  if(PythonActorPool::GetInstance()->Initialize(information->user_pool_initial_,
        information->user_pool_extend_) == false) {
    global::LogError("%s:%d (%s) 初始化玩家对象池失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // Npc对象池初始化
  if(PythonNpcPool::GetInstance()->Initialize(information->user_pool_initial_,
        information->user_pool_extend_) == false) {
    global::LogError("%s:%d (%s) 初始化 Npc对象池失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 事件回调
  if(this->actor_login_event_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 ActorLoginEventHandler 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->actor_logout_event_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 ActorLogoutEventHandler 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->npc_create_destory_event_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 NpcCreateDestoryEventHandler 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  return true;
}

void PythonExtension::Finalize() {
  this->npc_create_destory_event_.Finalize();
  this->actor_logout_event_.Finalize();
  this->actor_login_event_.Finalize();

  PythonEngine::GetInstance()->Finalize();

  PythonNpcPool::GetInstance()->Finalize();
  PythonNpcPool::ReleaseInstance();
  PythonActorPool::GetInstance()->Finalize();
  PythonActorPool::ReleaseInstance();
  PythonNpcManager::ReleaseInstance();
  PythonActorManager::ReleaseInstance();
}

void PythonExtension::Start() {}
void PythonExtension::Stop() {}

}  // namespace python

}  // namespace server

}  // namespace game

