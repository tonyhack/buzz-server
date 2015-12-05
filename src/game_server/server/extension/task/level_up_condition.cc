//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-10-11 01:45:12.
// File name: level_up_condition.cc
//
// Description:
//
#include "game_server/server/extension/task/level_up_condition.h"

#include "game_server/server/extension/task/task.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/extension/task/task_actor.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "gateway_server/protocol/gateway_task_protocol_types.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace task {

LevelUpCondition::LevelUpCondition() {}
LevelUpCondition::~LevelUpCondition() {}

bool LevelUpCondition::Initialize(Task *task, core::uint32 param1,
    core::uint32 param2, core::uint32 value) {
  if(Condition::Initialize(task, param1, param2, value) == false) {
    global::LogError("%s:%d (%s) 初始化 LevelUpCondition 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->require_lv_ = param1;

  return true;
}

void LevelUpCondition::Finalize() {
  Condition::Finalize();
}

bool LevelUpCondition::ExportConditionVar(core::uint32 &var, core::uint32 &value) {
  var = 0;
  if(this->task_ == NULL) {
    LOG_ERROR("当前的任务对象没有找到");
    return false;
  }
  TaskActor *task_actor = this->task_->GetActor();
  if(task_actor == NULL) {
    LOG_ERROR("获取玩家任务对象指针失败");
    return false;
  }
  GameActor *game_actor = task_actor->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("玩家对象没有找到");
    return false;
  }
  value = (core::uint32)game_actor->GetAttribute(entity::RoleAoiFields::LEVEL);

  return true;
}

bool LevelUpCondition::CheckCondition() {
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

  // 获取玩家对象
  GameActor *game_actor = task_actor->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("获取玩家对象失败");
    return false;
  }

  // 获取等级
  if(game_actor->GetAttribute(entity::RoleAoiFields::LEVEL) < this->require_lv_) {
    return false;
  }
  return true;
}

void LevelUpCondition::Synchronize() {
  if(this->task_ == NULL) {
    global::LogError("%s:%d (%s) 同步任务数据时 task_ 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  
  if(this->task_ == NULL) {
    LOG_ERROR("当前的任务对象没有找到");
    return ;
  }
  TaskActor *task_actor = this->task_->GetActor();
  if(task_actor == NULL) {
    LOG_ERROR("获取玩家任务对象指针失败");
    return ;
  }
  GameActor *game_actor = task_actor->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("玩家对象没有找到");
    return ;
  }
  // 同步任务变量
  gateway::protocol::TaskVar var;
  var.__set_type_(this->GetType());
  var.__set_id_(0);
  var.__set_value_(game_actor->GetAttribute(entity::RoleAoiFields::LEVEL));

  this->task_->SynchronizeVariable(var);
}

void LevelUpCondition::OnEvent(event::EventType::type type,
    const void *data, size_t size) {
  if(data == NULL) {
    global::LogError("%s:%d (%s) 参数 data 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  switch(type) {
    case event::EventType::EVENT_ACTOR_LEVEL_UP:
      this->OnEventLevelUp(data, size);
      break;
    default:
      break;
  }
}

void LevelUpCondition::OnEventLevelUp(const void *data, size_t size) {
  if(data == NULL || size != sizeof(event::EventActorLevelUp)) {
    global::LogError("%s:%d (%s) 参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  // 同步任务数据
   this->Synchronize();
}

}  // namespace task

}  // namespace server

}  // namespace game

