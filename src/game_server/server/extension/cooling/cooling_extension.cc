//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-25 17:43:34.
// File name: cooling_extension.cc
//
// Description:
// Define class CoolingExtension.
//

#include "game_server/server/extension/cooling/cooling_extension.h"

#include "game_server/server/extension/cooling/configure.h"
#include "game_server/server/extension/cooling/cooling_group_pool.h"
#include "game_server/server/extension/cooling/cooling_pool.h"
#include "game_server/server/extension/cooling/cooling_role_manager.h"
#include "game_server/server/extension/cooling/cooling_role_pool.h"
#include "game_server/server/game_server.h"
#include "global/logging.h"
#include "global/server_configure.h"

namespace game {

namespace server {

namespace cooling {

CoolingExtension::CoolingExtension() {}

CoolingExtension::~CoolingExtension() {}

bool CoolingExtension::Initialize() {
  // 冷却角色管理器
  if(CoolingRoleManager::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) 冷却角色管理器不存在",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 冷却对象池
  if(CoolingPool::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) 冷却对象池不存在",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 冷却角色对象池
  if(CoolingRolePool::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) 冷却角色对象池不存在",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 冷却组对象池
  if(CoolingGroupPool::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) 冷却组对象池不存在",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 配置对象
  if(Configure::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) 配置对象不存在",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 加载冷却配置文件
  if(Configure::GetInstance()->LoadConfigure(
        global::ServerConfigureSingleton::GetInstance().GetCoolingDir()) == false) {
    global::LogError("%s:%d (%s) 加载冷却配置失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 冷却对象池初始化
  if(CoolingPool::GetInstance()->Initialize(
        Configure::GetInstance()->GetCoolingPoolInitial().initial_number_,
        Configure::GetInstance()->GetCoolingPoolInitial().extend_number_) == false) {
    global::LogError("%s:%d (%s) 初始化冷却对象池失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 冷却组对象池初始化
  if(CoolingGroupPool::GetInstance()->Initialize(
        Configure::GetInstance()->GetCoolingGroupPoolInitial().initial_number_,
        Configure::GetInstance()->GetCoolingGroupPoolInitial().extend_number_) == false) {
    global::LogError("%s:%d (%s) 初始化冷却组对象池失败",
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

  if(CoolingRoleManager::GetInstance()->Initialize() == false) {
    LOG_ERROR("初始化 CoolingRolePool 失败");
    return false;
  }

  // 初始冷却玩家对象池失败
  if(CoolingRolePool::GetInstance()->Initialize(information->user_pool_initial_,
        information->user_pool_extend_) == false) {
    global::LogError("%s:%d (%s) 初始化冷却玩家对象池失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 冷却玩家对象登陆回调
  if(this->storage_cooling_login_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 StorageCoolingLoginResponse 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  // 冷却玩家对象保存回调
  if(this->storage_cooling_save_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 StorageCoolingSaveResponse 失败",
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
  if(this->actor_save_event_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 ActorSaveEventHandler 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->npc_create_event_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 NpcCreateEventHandler 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 请求回调
  if(this->req_check_in_cooling_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 RequestCheckInCooling 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->req_start_cooling_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 RequestStartCooling 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  return true;
}

void CoolingExtension::Finalize() {
  this->req_start_cooling_.Finalize();
  this->req_check_in_cooling_.Finalize();

  this->npc_create_event_.Finalize();
  this->actor_save_event_.Finalize();
  this->actor_logout_event_.Finalize();
  this->actor_login_event_.Finalize();

  this->storage_cooling_save_.Finalize();
  this->storage_cooling_login_.Finalize();

  CoolingRoleManager::GetInstance()->Finalize();
  CoolingRolePool::GetInstance()->Finalize();
  CoolingGroupPool::GetInstance()->Finalize();
  CoolingPool::GetInstance()->Finalize();

  Configure::ReleaseInstance();
  CoolingRoleManager::ReleaseInstance();
  CoolingRolePool::ReleaseInstance();
  CoolingGroupPool::ReleaseInstance();
  CoolingPool::ReleaseInstance();
}

void CoolingExtension::Start() {}

void CoolingExtension::Stop() {}

}  // namespace cooling

}  // namespace server

}  // namespace game

