//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-07 20:19:14.
// File name: effect_extension.cc
//
// Description:
// Define class EffectExtension.
//

#include "game_server/server/extension/effect/effect_extension.h"

#include "game_server/server/game_server.h"
#include "game_server/server/extension/effect/effect_pool.h"
#include "game_server/server/extension/effect/effect_role_manager.h"
#include "game_server/server/extension/effect/effect_role_pool.h"
#include "game_server/server/extension/effect/configure.h"
#include "global/logging.h"
#include "global/server_configure.h"

namespace game {

namespace server {

namespace effect {

EffectExtension::EffectExtension() {}
EffectExtension::~EffectExtension() {}

bool EffectExtension::Initialize() {
  // 效果对象池
  if(EffectPool::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) 效果对象池不存在",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 效果角色对象池
  if(EffectRolePool::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) 效果角色对象池不存在",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 效果角色管理器
  if(EffectRoleManager::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) 效果角色管理器不存在",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 配置对象
  if(Configure::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) 配置对象不存在",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 加载效果配置文件
  if(Configure::GetInstance()->LoadConfigure(
        global::ServerConfigureSingleton::GetInstance().GetEffectDir()) == false) {
    global::LogError("%s:%d (%s) 加载效果配置失败",
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

  // 效果对象池初始化
  if(EffectPool::GetInstance()->Initialize(information->user_pool_initial_,
        information->user_pool_extend_) == false) {
    global::LogError("%s:%d (%s) 初始化效果对象池失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 初始化效果玩家对象池失败
  if(EffectRolePool::GetInstance()->Initialize(information->user_pool_initial_,
        information->user_pool_extend_) == false) {
    global::LogError("%s:%d (%s) 初始化冷却玩家对象池失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 初始化 EffectRoleManager
  if(EffectRoleManager::GetInstance()->Initialize() == false) {
    LOG_ERROR("初始化 EffectRoleManager 失败");
    return false;
  }

  // 事件回调
  if(this->actor_logout_event_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 ActorLogoutEventHandler 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if (this->extension_load_event_.Initialize() == false) {
    LOG_ERROR("初始化 ExtensionLoadEventHandler 失败");
    return false;
  }
  if(this->npc_create_event_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 NpcCreateEventHandler 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 请求回调
  if(this->req_effect_start_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 RequestEffectStart 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->req_effect_end_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 RequestEffectEnd 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  return true;
}

void EffectExtension::Finalize() {
  this->req_effect_end_.Finalize();
  this->req_effect_start_.Finalize();

  this->npc_create_event_.Finalize();
  this->extension_load_event_.Finalize();
  this->actor_logout_event_.Finalize();

  EffectRoleManager::GetInstance()->Finalize();
  EffectRolePool::GetInstance()->Finalize();
  EffectPool::GetInstance()->Finalize();

  Configure::ReleaseInstance();
  EffectRoleManager::ReleaseInstance();
  EffectRolePool::ReleaseInstance();
  EffectPool::ReleaseInstance();
}

void EffectExtension::Start() {}
void EffectExtension::Stop() {}

}  // namespace effect

}  // namespace server

}  // namespace game

