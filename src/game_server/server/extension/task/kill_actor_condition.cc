
#include "game_server/server/extension/task/kill_actor_condition.h"

#include "game_server/server/request/game_playing_request_types.h"
#include "game_server/server/request/game_request_types.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_npc.h"
#include "game_server/server/game_npc_manager.h"
#include "game_server/server/extension/task/task.h"
#include "game_server/server/extension/task/task_configure.h"
#include "game_server/server/extension/task/configure.h"
#include "game_server/server/extension/task/task_actor.h"
#include "game_server/server/event/game_role_event_types.h"
#include "game_server/server/event/game_role_event_types.h"
#include "game_server/server/request/game_item_request_types.h"
#include "gateway_server/protocol/gateway_task_protocol_types.h"
#include "global/logging.h"
#include "global/types.h"

namespace game {

namespace server {

namespace task {

KillActorCondition::KillActorCondition() {}
KillActorCondition::~KillActorCondition() {}

bool KillActorCondition::Initialize(Task *task, core::uint32 param1,
    core::uint32 param2, core::uint32 value) {
  if(Condition::Initialize(task, param1, param2, value) == false) {
    global::LogError("%s:%d (%s) 初始化 KillActorCondition 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->require_id_ = param1;
  this->quired_num_= param2;
  this->count_ = value;

  return true;
}

void KillActorCondition::Finalize() {
  Condition::Finalize();
}

bool KillActorCondition::ExportConditionVar(
    core::uint32 &var, core::uint32 &value) { 
  // 获取任务玩家对象
  if(this->task_ == NULL) {
    LOG_ERROR("玩家的任务指针没有找到");
    return false;
  }
  TaskActor *task_actor = this->task_->GetActor();
  if(task_actor == NULL) {
    LOG_ERROR("任务玩家对象获取失败");
    return false;
  }

  var = this->require_id_;
  value = this->count_;

  return true;
}

bool KillActorCondition::CheckCondition() {
  if(this->task_ == NULL) {
    LOG_ERROR("玩家对象没有找到");
    return false;
  }

  // 获取任务玩家对象
  TaskActor *task_actor = this->task_->GetActor();
  if(task_actor == NULL) {
    LOG_ERROR("任务玩家对象获取失败");
    return false;
  }

  return this->count_ >= this->quired_num_;
}

void KillActorCondition::Synchronize() {
  if(this->task_ == NULL) {
    global::LogError("%s:%d (%s) 同步任务数据时 task_ 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  // 同步任务变量
  gateway::protocol::TaskVar var;
  var.__set_type_(this->GetType());
  var.__set_id_(this->require_id_);
  var.__set_value_(this->count_);

  this->task_->SynchronizeVariable(var);

}

void KillActorCondition::OnEvent(event::EventType::type type,
    const void *data, size_t size) {
  if(data == NULL) {
    global::LogError("%s:%d (%s) 参数 data 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  switch(type) {
    case event::EventType::EVENT_ROLE_KILLED:
      this->OnEvent(data, size);
      break;
    default:
      break;
  }
}

void KillActorCondition::OnEvent(const void *data, size_t size) {
  if(data == NULL || size != sizeof(event::EventRoleKilled)) {
    global::LogError("%s:%d (%s) 参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  event::EventRoleKilled *event = (event::EventRoleKilled *)data;

  if(event->attacker_type_ != entity::EntityType::TYPE_ACTOR ||
      event->type_ != entity::EntityType::TYPE_ACTOR) {
    return ;
  }

  // 获取任务玩家对象
  TaskActor *task_actor = this->task_->GetActor();
  if(task_actor == NULL) {
    LOG_ERROR("任务玩家对象获取失败");
    return ;
  }

  // 获取玩家对象
  GameActor *game_actor = task_actor->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("获取玩家对象失败");
    return ;
  }
  if(event->attacker_id_ != (core::int64)game_actor->GetActorID()) {
    return ;
  }
  // 获取玩家所在的副本
  request::RequestPlayingActorPlayingID request;
  request.__set_actor_(game_actor->GetActorID());
  if(ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_PLAYING_ACTOR_PLAYING_ID,
        &request, sizeof(request)) == -1) {
    LOG_ERROR("请求玩家所在的副本失败");
    return ;
  }
  if(this->require_id_ != request.playing_id_) {
    LOG_ERROR("副本不符合要求");
    return ;
  }

  if(this->quired_num_ <  this->count_) {
    this->count_++;
    // 同步任务数据
    this->Synchronize();
  }
}

void KillActorCondition::Clear() {
}

}  // namespace task

}  // namespace server

}  // namespace game

