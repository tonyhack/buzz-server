//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-07 20:19:14.
// File name: buff_extension.cc
//
// Description:
// Define class BuffExtension.
//

#include "game_server/server/extension/buff/buff_extension.h"

#include "game_server/server/game_server.h"
#include "game_server/server/extension/buff/buff_pool.h"
#include "game_server/server/extension/buff/buff_role_manager.h"
#include "game_server/server/extension/buff/buff_role_pool.h"
#include "game_server/server/extension/buff/condition_pool.h"
#include "game_server/server/extension/buff/configure.h"
#include "global/logging.h"
#include "global/server_configure.h"

namespace game {

namespace server {

namespace buff {

BuffExtension::BuffExtension() {}
BuffExtension::~BuffExtension() {}

bool BuffExtension::Initialize() {
  // Buff对象池
  if(BuffPool::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) Buff对象池不存在",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // Buff角色对象池
  if(BuffRolePool::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) Buff角色对象池不存在",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // Buff角色管理器
  if(BuffRoleManager::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) Buff角色管理器不存在",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(ConditionPool::GetInstance() == NULL) {
    LOG_ERROR("ConditionPool 不存在");
    return false;
  }

  // 配置对象
  if(Configure::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) 配置对象不存在",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 加载Buff配置文件
  if(Configure::GetInstance()->LoadConfigure(
        global::ServerConfigureSingleton::GetInstance().GetBuffFile()) == false) {
    global::LogError("%s:%d (%s) 加载Buff配置失败",
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

  // 初始化Buff玩家对象池失败
  if(BuffRolePool::GetInstance()->Initialize(information->user_pool_initial_,
        information->user_pool_extend_) == false) {
    global::LogError("%s:%d (%s) 初始化冷却玩家对象池失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(BuffRoleManager::GetInstance()->Initialize() == false) {
    LOG_ERROR("初始化 BuffRoleManager 失败");
    return false;
  }

  // Buff对象池初始化
  if(BuffPool::GetInstance()->Initialize(
        Configure::GetInstance()->GetBuffPoolInitial().initial_number_,
        Configure::GetInstance()->GetBuffPoolInitial().extend_number_) == false) {
    global::LogError("%s:%d (%s) 初始化Buff对象池失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(ConditionPool::GetInstance()->Initialize(
        Configure::GetInstance()->GetConditionPoolInitial().initial_number_,
        Configure::GetInstance()->GetConditionPoolInitial().extend_number_) == false) {
    LOG_ERROR("初始化 ConditionPool 不存在");
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
    global::LogError("%s:%d (%s) 初始化 ActorsaveEventHandler 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->attr_change_event_.Initialize() == false) {
    LOG_ERROR("初始化 AttributeChangeEventHandler 失败");
    return false;
  }
  if(this->npc_create_event_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 NpcCreateEventHandler 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->extension_load_event_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 ExtensionLoadEventHandler 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->role_basic_status_change_event_.Initialize() == false) {
    LOG_ERROR("初始化 RoleBasicStatusChangeHandler 失败");
    return false;
  }
  if(this->scene_role_create_event_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 SceneRoleCreateEventHandler 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->skill_event_.Initialize() == false) {
    LOG_ERROR("初始化 SkillEventHandler 失败");
    return false;
  }
  if(this->buff_event_.Initialize() == false) {
    LOG_ERROR("初始化 BuffEventHandler 失败");
    return false;
  }

  // 请求回调
  if(this->req_buff_handler_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 RequestBuffHandler 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 冷对象登陆回调
  if(this->storage_buff_login_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 StorageCoolingLoginResponse 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  // 冷却玩家对象保存回调
  if(this->storage_buff_save_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 StorageCoolingSaveResponse 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  return true;
}

void BuffExtension::Finalize() {
  this->req_buff_handler_.Finalize();

  this->actor_logout_event_.Finalize();
  this->actor_login_event_.Finalize();
  this->actor_save_event_.Finalize();
  this->attr_change_event_.Finalize();
  this->extension_load_event_.Finalize();
  this->role_basic_status_change_event_.Finalize();
  this->scene_role_create_event_.Finalize();
  this->skill_event_.Finalize();
  this->buff_event_.Finalize();

  this->storage_buff_save_.Finalize();
  this->storage_buff_login_.Finalize();

  ConditionPool::GetInstance()->Finalize();

  BuffPool::GetInstance()->Finalize();
  BuffRoleManager::GetInstance()->Finalize();
  BuffRolePool::GetInstance()->Finalize();

  Configure::ReleaseInstance();
  BuffPool::ReleaseInstance();
  BuffRoleManager::ReleaseInstance();
  BuffRolePool::ReleaseInstance();
}

void BuffExtension::Start() {}
void BuffExtension::Stop() {}

}  // namespace buff

}  // namespace server

}  // namespace game

