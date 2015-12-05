//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-06 14:05:54.
// File name: skill_extension.cc
//
// Description:
// Define class SkillExtension.
//

#include "game_server/server/extension/skill/skill_extension.h"

#include "game_server/server/game_server.h"
#include "game_server/server/extension/skill/configure.h"
#include "game_server/server/extension/skill/skill_configure.h"
#include "game_server/server/extension/skill/skill_pool.h"
#include "game_server/server/extension/skill/skill_role_manager.h"
#include "game_server/server/extension/skill/skill_role_pool.h"
#include "global/logging.h"
#include "global/server_configure.h"
#include "global/server_informations.h"

namespace game {

namespace server {

namespace skill {

SkillExtension::SkillExtension() {}
SkillExtension::~SkillExtension() {}

bool SkillExtension::Initialize() {
  // 配置文件 
  if(Configure::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) 技能配置不存在",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 技能角色管理器
  if(SkillRoleManager::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) 技能角色管理器不存在",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 技能角色对象池
  if(SkillRolePool::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) 技能角色对象池不存在",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 技能对象池
  if(SkillPool::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) 技能对象池不存在",
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
  if(Configure::GetInstance()->LoadConfigure(
        global::ServerConfigureSingleton::GetInstance().GetConfigueDir() +
        global::ServerConfigureSingleton::GetInstance().GetSkillFile()) == false) {
    global::LogError("%s:%d (%s) 加载 skill 配置失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 技能角色对象池初始化
  if(SkillRolePool::GetInstance()->Initialize(
        Configure::GetInstance()->role_pool_initial_,
        Configure::GetInstance()->role_pool_extend_) == false) {
    global::LogError("%s:%d (%s) 初始化技能角色对象池失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 技能对象池初始化
  if(SkillPool::GetInstance()->Initialize(
        Configure::GetInstance()->skill_pool_initial_,
        Configure::GetInstance()->skill_pool_extend_) == false) {
    global::LogError("%s:%d (%s) 初始化技能对象池失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 事件回调
  if (this->actor_login_event_.Initialize() == false) {
    LOG_ERROR("初始化 ActorLogoutEventHandler 失败");
    return false;
  }
  if(this->actor_logout_event_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 ActorLogoutEventHandler 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->npc_create_event_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 NpcCreateEventHandler 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->status_change_event_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 BasicStatusChangeEventHandler 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->change_location_event_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 ChangeLocationEventHandler 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->threat_event_.Initialize() == false) {
    LOG_ERROR("初始化 ThreatEventHandler 失败");
    return false;
  }
  if(this->common_status_event_.Initialize() == false) {
    LOG_ERROR("初始化 RoleCommonStatusEventHandler 失败");
    return false;
  }
  if(this->scene_role_create_event_.Initialize() == false) {
    LOG_ERROR("初始化 SceneRoleCreateEventHandler 失败");
    return false;
  }

  // 请求回调
  if(this->request_global_buff_.Initialize() == false) {
    LOG_ERROR("初始化 RequestGlobalBuff 失败");
    return false;
  }
  if(this->request_skill_alter_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 RequestSkillAlter 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->request_skill_hurt_.Initialize() == false) {
    LOG_ERROR("初始化 RequestSkillHurt 失败");
    return false;
  }
  if(this->request_stand_skill_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 RequestStandSkill 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->request_temp_skill_.Initialize() == false) {
    LOG_ERROR("初始化 RequestTempSkill 失败");
    return false;
  }
  if(this->request_use_skill_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 RequestUseSkill 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if (this->request_set_fighting_status_.Initialize() == false) {
    LOG_ERROR("初始化 RequestSetFightingStatus 失败");
    return false;
  }

  // 消息回调
  if(this->skill_message_handler_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 SkillMessageHandler 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  return true;
}

void SkillExtension::Finalize() {
  this->skill_message_handler_.Finalize();

  this->request_set_fighting_status_.Finalize();
  this->request_use_skill_.Finalize();
  this->request_temp_skill_.Finalize();
  this->request_stand_skill_.Finalize();
  this->request_skill_hurt_.Finalize();
  this->request_skill_alter_.Finalize();
  this->request_global_buff_.Finalize();

  this->npc_create_event_.Finalize();
  this->actor_login_event_.Finalize();
  this->actor_logout_event_.Finalize();
  this->status_change_event_.Finalize();
  this->change_location_event_.Finalize();
  this->threat_event_.Finalize();
  this->common_status_event_.Finalize();
  this->scene_role_create_event_.Finalize();

  SkillPool::GetInstance()->Finalize();
  SkillRolePool::GetInstance()->Finalize();

  SkillPool::ReleaseInstance();
  SkillRolePool::ReleaseInstance();
  Configure::ReleaseInstance();
}

void SkillExtension::Start() {}

void SkillExtension::Stop() {}

}  // namespace skill

}  // namespace server

}  // namespace game

