//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-11-03 22:08:39.
// File name: bar_extension.cc
//
// Description:
//
#include "game_server/server/extension/bar/bar_extension.h"

#include "game_server/server/game_server.h"
#include "game_server/server/extension/bar/bar_actor_manager.h"
#include "game_server/server/extension/bar/bar_actor_pool.h"
#include "game_server/server/extension/bar/configure.h"
#include "global/logging.h"
#include "global/server_configure.h"
#include "global/server_informations.h"

namespace game {

namespace server {

namespace bar {

BarExtension::BarExtension() {}
BarExtension::~BarExtension() {}

bool BarExtension::Initialize() {
  // 配置文件
  if(Configure::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) 进度条配置不存在",
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

  // 加载配置文件
  if(Configure::GetInstance()->LoadBarConfigure(
        global::ServerConfigureSingleton::GetInstance().GetBarFile()) == false) {
    global::LogError("%s:%d (%s) 加载进度条配置失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 进度条玩家管理器
  if(BarActorManager::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) 进度条玩家管理器不存在",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 进度条玩家对象池
  if(BarActorPool::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) 进度条玩家对象池不存在",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(BarActorPool::GetInstance()->Initialize(information->user_pool_initial_,
        information->user_pool_extend_) == false) {
    LOG_ERROR("初始化进度条玩家对象池失败");
  }

  // 事件
  if(this->login_event_handler_.Initialize() == false) {
    LOG_ERROR("玩家 ActorLoginEventHandler 初始化失败");
    return false;
  }
  if(this->logout_event_handler_.Initialize() == false) {
    LOG_ERROR("玩家 ActorLoginEventHandler 初始化失败");
    return false;
  }
  if(this->move_event_handler_.Initialize() == false) {
    LOG_ERROR("玩家 ActorMoveEventHandler 初始化失败");
    return false;
  }
  if(this->hurt_event_handler_.Initialize() == false) {
    LOG_ERROR("玩家 ActorHurtEventHandler 初始化失败");
    return false;
  }
  if(this->use_skill_event_handler_.Initialize() == false) {
    LOG_ERROR("玩家 ActorUseSkillEventHandler 初始化失败");
    return false;
  }
  if(this->use_item_event_handler_.Initialize() == false) {
    LOG_ERROR("玩家 ActorUseItemEventHandler 初始化失败");
    return false;
  }
  if(this->die_event_handler_.Initialize() == false) {
    LOG_ERROR("玩家 ActorDieEventHandler 初始化失败");
    return false;
  }
  
  // 消息
  if(this->message_handler_.Initialize() == false) {
    LOG_ERROR("玩家 BarMessageHandler 初始化失败");
    return false;
  }

  // 请求
  if(this->request_handler_.Initialize() == false) {
    LOG_ERROR("玩家 RequestBarHandler 初始化失败");
    return false;
  }  

  return true;
}

void BarExtension::Finalize() {
 
  this->request_handler_.Finalize();

  this->logout_event_handler_.Finalize();
  this->login_event_handler_.Finalize();
  this->move_event_handler_.Finalize();
  this->hurt_event_handler_.Finalize();
  this->use_skill_event_handler_.Finalize();
  this->use_item_event_handler_.Finalize();
  this->die_event_handler_.Finalize();
  
  this->message_handler_.Finalize();

  BarActorPool::GetInstance()->Finalize();

  BarActorPool::ReleaseInstance();
  BarActorManager::ReleaseInstance();

  Configure::ReleaseInstance();
}

void BarExtension::Start() {}
void BarExtension::Stop() {}

}  // namespace bar

}  // namespace server

}  // namespace game

