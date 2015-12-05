//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-06-10 04:25:26.
// File name: transport_extension.cc
//
// Description:
// Define class TransportExtension.
//

#include "game_server/server/extension/transport/transport_extension.h"

#include "game_server/server/game_server.h"
#include "game_server/server/extension/transport/transport_actor_manager.h"
#include "game_server/server/extension/transport/transport_actor_pool.h"
#include "game_server/server/extension/transport/transport_destory_checker.h"
#include "game_server/server/extension/transport/transport_npc_manager.h"
#include "game_server/server/extension/transport/transport_npc_pool.h"
#include "global/logging.h"
#include "global/server_configure.h"
#include "global/server_informations.h"
#include "global/configure/configure.h"

namespace game {

namespace server {

namespace transport {

bool TransportExtension::Initialize() {
  const global::GameServerInformation *information =
    global::ServerConfigureSingleton::GetInstance().GetGameServers(
        GameServerSingleton::GetInstance().GetSignature().id_);
  if(information == NULL) {
    LOG_ERROR("获取当前服务器配置失败");
    return false;
  }

  if (TransportDestoryChecker::GetInstance() == NULL) {
    LOG_ERROR("获取 TransportDestoryChecker 失败");
    return false;
  }

  if (TransportActorManager::GetInstance() == NULL) {
    LOG_ERROR("获取 TransportActorManager 失败");
    return false;
  }

  if (TransportActorPool::GetInstance() == NULL) {
    LOG_ERROR("获取 TransportActorPool 失败");
    return false;
  }

  if (TransportNpcManager::GetInstance() == NULL) {
    LOG_ERROR("获取 TransportNpcManager 失败");
    return false;
  }

  if (TransportNpcPool::GetInstance() == NULL) {
    LOG_ERROR("获取 TransportNpcPool 失败");
    return false;
  }

  if (TransportDestoryChecker::GetInstance()->Initialize() == false) {
    LOG_ERROR("初始化 TransportDestoryChecker 失败");
    return false;
  }

  if (TransportActorPool::GetInstance()->Initialize(information->user_pool_initial_,
        information->user_pool_extend_) == false) {
    LOG_ERROR("初始化 TransportActorPool 失败");
    return false;
  }

  if (TransportNpcPool::GetInstance()->Initialize(information->user_pool_initial_,
        information->user_pool_extend_) == false) {
    LOG_ERROR("初始化 TransportNpcPool 失败");
    return false;
  }

  if (this->request_handler_.Initialize() == false) {
    LOG_ERROR("初始化 request_handler_ 失败");
    return false;
  }

  if (this->event_handler_.Initialize() == false) {
    LOG_ERROR("初始化 event_handler_ 失败");
    return false;
  }

  return true;
}

void TransportExtension::Finalize() {
  this->event_handler_.Finalize();
  this->request_handler_.Finalize();

  TransportNpcPool::GetInstance()->Finalize();
  TransportActorPool::GetInstance()->Finalize();
  TransportDestoryChecker::GetInstance()->Finalize();

  TransportNpcPool::ReleaseInstance();
  TransportNpcManager::ReleaseInstance();
  TransportActorPool::ReleaseInstance();
  TransportActorManager::ReleaseInstance();
  TransportDestoryChecker::ReleaseInstance();
}

void TransportExtension::Start() {}

void TransportExtension::Stop() {}

}  // namespace transport

}  // namespace server

}  // namespace game

