
#include "game_server/server/extension/welfare/welfare_extension.h"

#include "game_server/server/extension/welfare/facede_request.h"
#include "game_server/server/extension/welfare/configure.h"
#include "game_server/server/extension/welfare/welfare_actor_manager.h"
#include "game_server/server/extension/welfare/task_pool.h"
#include "game_server/server/extension/welfare/condition_pool.h"
#include "game_server/server/game_server.h"
#include "game_server/server/extension/welfare/welfare_actor_pool.h"
#include "global/server_informations.h"
#include "global/server_configure.h"


namespace game {

namespace server {

namespace welfare {

WelfareExtension::WelfareExtension() {}
WelfareExtension::~WelfareExtension() {}

bool WelfareExtension::Initialize() {
  // 配置文件初始化
  if(Configure::GetInstance() == NULL) {
    LOG_ERROR("福利配置不存在");
    return false;
  }

  // 初始化成就玩家对象
  if(WelfareActorManager::GetInstance() == NULL) {
    LOG_ERROR("初始化福利对象管理器失败");
    return false;
  }

  // 初始化成就玩家对象池
  if(WelfareActorPool::GetInstance() == NULL) {
    LOG_ERROR("初始化成就对象内存池失败");
    return false;
  }

  // 初始化成就任务池
  if(TaskPool::GetInstance() == NULL) {
    LOG_ERROR("初始化成就任务池 失败");
    return false;
  }

  // 初始化条件对象池
  if(ConditionPool::GetInstance() == NULL) {
    LOG_ERROR("初始化条件对象池 失败");
    return false;
  }

  // 初始化外观请求
  if(FacedeRequest::GetInstance() == NULL) {
    LOG_ERROR("初始化外观请求 失败");
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
        global::ServerConfigureSingleton::GetInstance().GetWelfarefile()) == false) {
    global::LogError("%s:%d (%s)(%s) 加载任务配置失败",
        __FILE__, __LINE__, __FUNCTION__, global::ServerConfigureSingleton::GetInstance().GetWelfarefile().c_str());
    return false;
  }

  // 成就玩家对象池初始化
  if(WelfareActorPool::GetInstance()->Initialize(information->user_pool_initial_,
        information->user_pool_extend_) == false) {
    LOG_ERROR("成就玩家对象池初始化 失败");
    return false;
  }

  if(TaskPool::GetInstance()->Initialize(Configure::GetInstance()->task_initial_number_,
        Configure::GetInstance()->task_extend_number_) == false) {
    LOG_ERROR("成就任务初始化失败");
    return false;
  }

  if(ConditionPool::GetInstance()->Initialize(Configure::GetInstance()->condition_initial_number_,
        Configure::GetInstance()->condition_extend_number_) == false) {
    LOG_ERROR("成就条件初始化失败");
    return false;
  }

  // 事件初始化
  if(this->attr_event_.Initialize() == false) {
    LOG_ERROR("事件 AttrEventHandler 初始化失败");
    return false;
  }

  if(this->item_event_.Initialize() == false) {
    LOG_ERROR("事件 ItemEventHandler 初始化失败");
    return false;
  }

  if(this->die_event_.Initialize() == false) {
    LOG_ERROR("事件 RoleDieEventHandler 初始化失败");
    return false;
  }

  if(this->skill_event_.Initialize() == false) {
    LOG_ERROR("事件 SkillEventHandler 初始化失败");
    return false;
  }

  if(this->actor_event_.Initialize() == false) {
    LOG_ERROR("事件 ActorEventHandler 初始化失败");
    return false;
  }

  if(this->extension_event_.Initialize() == false) {
    LOG_ERROR("事件 ActorExtensionEventHandler 初始化失败");
    return false;
  }

  if(this->logout_event_.Initialize() == false) {
    LOG_ERROR("事件 ActorLogoutEventHandler 初始化失败");
    return false;
  }

  if(this->response_.Initialize() == false) {
    LOG_ERROR(" StorageLoginResponse 初始化失败");
    return false;
  }

  if(this->daily_clean_.Initialize() == false) {
    LOG_ERROR(" ActorDailyCleanEventHandler 初始化失败 ");
    return false;
  }

  if(this->scene_event_.Initialize() == false) {
    LOG_ERROR(" ActorComingEventHandler 初始化失败");
    return false;
  }

  // 消息初始化
  if(this->achieve_message_.Initialize() == false) {
    LOG_ERROR(" 消息 ActorMessageHandler 初始化失败");
    return false;
  }

  // 请求初始化
  if(this->request_.Initialize() == false) {
    LOG_ERROR("请求 RequestHandler 初始化失败");
    return false;
  }

  return true;
}

void WelfareExtension::Finalize() {
  ConditionPool::GetInstance()->Finalize();
  TaskPool::GetInstance()->Finalize();
  WelfareActorPool::GetInstance()->Finalize();

  this->attr_event_.Finalize();
  this->item_event_.Finalize();
  this->die_event_.Finalize();
  this->skill_event_.Finalize();
  this->actor_event_.Finalize();
  this->response_.Finalize();
  this->logout_event_.Finalize();
  this->extension_event_.Finalize();
  this->daily_clean_.Finalize();
  this->scene_event_.Finalize();
  this->request_.Finalize();

  this->achieve_message_.Finalize();

  ConditionPool::GetInstance()->ReleaseInstance();
  TaskPool::GetInstance()->ReleaseInstance();
  WelfareActorPool::GetInstance()->ReleaseInstance();
  WelfareActorManager::GetInstance()->ReleaseInstance();
}

void WelfareExtension::Start() {

}

void WelfareExtension::Stop() {

}

}  // namespace game

}  // namespace server

}  // namespace welfare
