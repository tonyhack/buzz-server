//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-06 13:37:03.
// File name: task_extension.cc
//
// Description:
// Define class TaskExtension.
//

#include "game_server/server/extension/task/task_extension.h"

#include "game_server/server/game_server.h"
#include "game_server/server/extension/task/condition_pool.h"
#include "game_server/server/extension/task/configure.h"
#include "game_server/server/extension/task/facade_request.h"
#include "game_server/server/extension/task/task_configure.h"
#include "game_server/server/extension/task/task_builder.h"
#include "game_server/server/extension/task/task_pool.h"
#include "game_server/server/extension/task/task_actor_manager.h"
#include "game_server/server/extension/task/task_actor_pool.h"
#include "global/logging.h"
#include "global/server_configure.h"
#include "global/server_informations.h"

namespace game {

namespace server {

namespace task {

TaskExtension::TaskExtension() {}
TaskExtension::~TaskExtension() {}

bool TaskExtension::Initialize() {
  // 配置文件
  if(Configure::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) 任务配置不存在",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 任务玩家管理器
  if(TaskActorManager::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) 任务玩家管理器不存在",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 任务玩家对象池
  if(TaskActorPool::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) 任务玩家对象池不存在",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 条件对象池
  if(ConditionPool::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) 任务条件对象池不存在",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 任务对象池
  if(TaskPool::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) 任务对象池不存在",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 任务创建器
  if(TaskBuilder::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) 任务创建器不存在",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 外观请求
  if(FacadeRequest::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) 任务外观请求不存在",
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
        global::ServerConfigureSingleton::GetInstance().GetTaskFile()) == false) {
    global::LogError("%s:%d (%s) 加载任务配置失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 任务玩家对象池初始化
  if(TaskActorPool::GetInstance()->Initialize(information->user_pool_initial_,
        information->user_pool_extend_) == false) {
    global::LogError("%s:%d (%s) 初始化任务玩家对象池失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 条件对象池初始化
  if(ConditionPool::GetInstance()->Initialize(
        Configure::GetInstance()->condition_initial_number_,
        Configure::GetInstance()->condition_extend_number_) == false) {
    global::LogError("%s:%d (%s) 初始化任务条件对象池失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 任务对象池初始化
  if(TaskPool::GetInstance()->Initialize(
        Configure::GetInstance()->task_initial_number_,
        Configure::GetInstance()->task_extend_number_) == false) {
    global::LogError("%s:%d (%s) 初始化任务对象池失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 任务创建器初始化
  if(TaskBuilder::GetInstance()->Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化任务创建器失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->actor_login_event_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 ActorExtensionEventHandler 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  // 事件回调
  if(this->dailyHandler_event_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 ActorDailyCleanEventHandler 失败",
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
  if(this->role_killed_event_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 RoleKilledEventHandler 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->soul_appear_event_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 SoulAppearEventHandler 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->item_event_.Initialize() == false) {
    LOG_ERROR("初始化 ItemEventHandler 失败");
    return false;
  }
  if(this->soul_event_.Initialize() == false) {
    LOG_ERROR("初始化 SoulEventHandler 失败");
    return false;
  }
  if(this->attr_event_.Initialize() == false) {
    LOG_ERROR("初始化 AttrEventHandler 失败");
    return false;
  }
  if(this->playing_event_.Initialize() == false) {
    LOG_ERROR("初始化 PlayingEventHandler 失败");
    return false;
  }
  if(this->functionality_event_.Initialize() == false) {
    LOG_ERROR("初始化 FunctionalityEventHandler 失败");
    return false;
  }
  if(this->actor_attr_event_.Initialize() == false) {
    LOG_ERROR("初始化 ActorAttrEventHandler 失败");
    return false;
  }
  if(this->scene_event_.Initialize() == false) {
    LOG_ERROR("初始化 SceneEventHandler 失败");
    return false;
  }
  if(this->homeland_event_.Initialize() == false) {
    LOG_ERROR("初始化 TaskHomelandEventHandler 失败");
    return false;
  }

  // 消息回调
  if(this->task_message_handler_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 TaskMessageHandler 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->circle_meesage_handler_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 TaskCircleMessageHandler 失败",
       __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->trip_messagme_handler_.Initialize() == false) {
    LOG_ERROR("初始化 TaskTripMessageHandler 失败");
    return false;
  }
  if(this->daily_message_handler_.Initialize() == false) {
    LOG_ERROR("初始化 TaskDailyMessageHandler 失败");
    return false;
  }
  if (this->transport_message_handler_.Initialize() == false) {
    LOG_ERROR("初始化 TaskTransportMessageHandler 失败");
    return false;
  }
  if(this->change_vip_event_.Initialize() == false) {
    LOG_ERROR("初始化 ChangeVipEventHandler 失败");
    return false;
  }
  if (this->transport_event_.Initialize() == false) {
    LOG_ERROR("初始化 TransportEventHandler 失败");
    return false;
  }

  // 存储过程回调
  if(this->storage_task_login_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 StorageTaskLoginResponse 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  
  // 请求 
  if(this->request_handler_.Initialize() == false) {
    LOG_ERROR("请求任务 RequestTaskHandler 初始化失败");
    return false;
  }
  if(this->request_layer_handler_.Initialize() == false) {
    LOG_ERROR("请求纹章 RequestTaskLayer 初始化");
    return false;
  }

  // 初始化NPC管理器
  if(this->npcs_.Initialize() == false) {
    LOG_ERROR("初始化 NPC SummonNpcManager 管理器失败");
    return false;
  }

  return true;
}

void TaskExtension::Finalize() {
  this->npcs_.Finalize();
  this->storage_task_login_.Finalize();
  
  this->task_message_handler_.Finalize();
  this->task_message_handler_.Finalize();
  this->trip_messagme_handler_.Finalize();
  this->daily_message_handler_.Finalize();
  this->transport_message_handler_.Finalize();
 
  this->functionality_event_.Finalize();
  this->playing_event_.Finalize();
  this->attr_event_.Finalize();
  this->item_event_.Finalize();
  this->soul_event_.Finalize();
  this->soul_appear_event_.Finalize();
  this->role_killed_event_.Finalize();
  this->actor_login_event_.Finalize();
  this->actor_logout_event_.Finalize();
  this->actor_save_event_.Finalize();
  this->dailyHandler_event_.Finalize();
  this->actor_attr_event_.Finalize();
  this->scene_event_.Finalize();
  this->homeland_event_.Finalize();
  this->change_vip_event_.Finalize();
  this->transport_event_.Finalize();

  this->request_handler_.Finalize();
  this->request_layer_handler_.Finalize();

  TaskBuilder::GetInstance()->Finalize();
  TaskPool::GetInstance()->Finalize();
  ConditionPool::GetInstance()->Finalize();
  TaskActorPool::GetInstance()->Finalize();

  FacadeRequest::ReleaseInstance();
  TaskBuilder::ReleaseInstance();
  TaskPool::ReleaseInstance();
  ConditionPool::ReleaseInstance();
  TaskActorPool::ReleaseInstance();
  TaskActorManager::ReleaseInstance();

  Configure::ReleaseInstance();
}

void TaskExtension::Start() {}
void TaskExtension::Stop() {}

}  // namespace task

}  // namespace server

}  // namespace game

