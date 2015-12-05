//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-09 19:50:54.
// File name: ai_extension.cc
//
// Description:
// Define AiExtension.
//

#include "game_server/server/extension/ai/ai_extension.h"

#include "game_server/server/extension/ai/ai_controller.h"
#include "game_server/server/extension/ai/behaviour_factory.h"
#include "game_server/server/extension/ai/behaviour_node_factory.h"
#include "game_server/server/extension/ai/behaviour_tree_manager.h"
#include "game_server/server/extension/ai/black_board_manager.h"
#include "game_server/server/extension/ai/black_board_pool.h"
#include "game_server/server/extension/ai/ai_actor_manager.h"
#include "game_server/server/extension/ai/ai_actor_pool.h"
#include "game_server/server/game_server.h"
#include "global/logging.h"
#include "global/server_configure.h"

namespace game {

namespace server {

namespace ai {

AiExtension::AiExtension() {}
AiExtension::~AiExtension() {}

bool AiExtension::Initialize() {
  // BlackBoard 对象池
  if(BlackBoardPool::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) BlackBoard 对象池不存在",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // BlackBoard 管理器
  if(BlackBoardManager::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) BlackBoard 管理器不存在",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 行为工厂
  if(ActionFactory::GetInstance() == NULL ||
      ConditionFactory::GetInstance() == NULL ||
      DecisionFactory::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) 行为工场 不存在",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 行为树结点工厂
  if(BehaviourNodeFactory::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) BehaviourNodeFactory 不存在",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 行为树管理器
  if(BehaviourTreeManager::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) BehaviourTreeManager 不存在",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // Ai 控制器
  if(AiController::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) AiController 不存在",
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

  // 初始化 BlackBoardManager
  if(BlackBoardManager::GetInstance()->Initialize() == false) {
    LOG_ERROR("初始化 BlackBoardManager 失败");
    return false;
  }

  // Npc对象池初始化
  if(BlackBoardPool::GetInstance()->Initialize(information->user_pool_initial_,
        information->user_pool_extend_) == false) {
    global::LogError("%s:%d (%s) 初始化 BlackBoard 对象池失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(BehaviourTreeManager::GetInstance()->Initialize(
      global::ServerConfigureSingleton::GetInstance().GetConfigueDir() + 
      global::ServerConfigureSingleton::GetInstance().GetAiFile()) == false) {
    global::LogError("%s:%d (%s) 初始化 BehaviourTree 管理器失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  
  // 玩家对象池初始化
  if(AiActorPool::GetInstance()->Initialize(information->user_pool_initial_,
        information->user_pool_extend_) == false) {
     global::LogError("%s:%d (%s) 初始化 AiActorPool 对象池失败",
         __FILE__, __LINE__, __FUNCTION__);
     return false;
  }
  if(AiActorManager::GetInstance()->Initialize() == false) {
     global::LogError("%s:%d (%s) 初始化 AiActorManager 失败",
        __FILE__, __LINE__, __FUNCTION__);
     return false;
  }

  // Ai 控制器
  if(AiController::GetInstance()->Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 AiController 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 事件回调
  if(this->npc_create_destory_event_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 NpcCreateDestoryEventHandler 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->role_coming_leaving_event_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 RoleComingLeavingEventHandler 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->role_basic_status_change_event_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 RoleBasicStatusChangeHandler 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->ai_event_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 AiEventHandler 失败",
       __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if (this->request_ai_handler_.Initialize() == false) {
    LOG_ERROR("初始化 RequestAiHandler 失败");
    return false;
  }
  if(this->req_add_threat_.Initialize() == false) {
    LOG_ERROR("初始化 RequestAddThreat 失败");
    return false;
  }

  return true;
}

void AiExtension::Finalize() {
  this->req_add_threat_.Finalize();
  this->request_ai_handler_.Finalize();

  this->role_basic_status_change_event_.Finalize();
  this->role_coming_leaving_event_.Finalize();
  this->npc_create_destory_event_.Finalize();
  this->ai_event_.Finalize();

  AiActorManager::GetInstance()->Finalize();
  AiActorPool::GetInstance()->Finalize();

  AiController::GetInstance()->Finalize();
  BehaviourTreeManager::GetInstance()->Finalize();
  BlackBoardManager::GetInstance()->Finalize();
  BlackBoardPool::GetInstance()->Finalize();

  AiController::ReleaseInstance();
  BehaviourTreeManager::ReleaseInstance();
  BehaviourNodeFactory::ReleaseInstance();
  ActionFactory::ReleaseInstance();
  ConditionFactory::ReleaseInstance();
  DecisionFactory::ReleaseInstance();
  BlackBoardManager::ReleaseInstance();
  BlackBoardPool::ReleaseInstance();
}

void AiExtension::Start() {
  AiController::GetInstance()->Start();
}

void AiExtension::Stop() {
  AiController::GetInstance()->Stop();
}

}  // namespace ai

}  // namespace server

}  // namespace game

