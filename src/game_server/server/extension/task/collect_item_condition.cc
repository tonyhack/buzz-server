//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-10-10 23:30:38.
// File name: collect_item_condition.cc
//
// Description:
//

#include "game_server/server/extension/task/collect_item_condition.h"

#include "game_server/server/extension/task/task.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/extension/task/task_actor.h"
#include "game_server/server/request/game_item_request_types.h"
#include "gateway_server/protocol/gateway_task_protocol_types.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace task {

CollectItemCondition::CollectItemCondition() {}
CollectItemCondition::~CollectItemCondition() {}

bool CollectItemCondition::Initialize(Task *task, core::uint32 param1,
    core::uint32 param2, core::uint32 value) {
  if(Condition::Initialize(task, param1, param2, value) == false) {
    global::LogError("%s:%d (%s) 初始化 CollectItemCondition 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->require_id_ = param1;
  this->count_ = param2;

  return true;
}

void CollectItemCondition::Finalize() {
  Condition::Finalize();
}

bool CollectItemCondition::ExportConditionVar(core::uint32 &var, core::uint32 &value) {
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

  request::RequestGetPacketItemNumber request_num;
  request_num.__set_actor_id_(game_actor->GetActorID());
  request_num.__set_item_template_(this->require_id_);
  request_num.__set_bind_(entity::BindRequireType::BOTH);
  if(ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_GET_PAKCET_ITEM_NUMBER,
        &request_num, sizeof(request_num)) == -1) {
    LOG_ERROR("请求背包指定道具数量出错,玩家[%lu],道具[%d]", game_actor->GetActorID(), this->require_id_);
    return false;
  }

  var = this->require_id_;
  value = (core::uint32)request_num.number_;
  return true;
}

bool CollectItemCondition::CheckCondition() {
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

  request::RequestGetPacketItemNumber request_num;
  request_num.__set_actor_id_(game_actor->GetActorID());
  request_num.__set_item_template_(this->require_id_);
  request_num.__set_bind_(entity::BindRequireType::BOTH);
  if(ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_GET_PAKCET_ITEM_NUMBER,
        &request_num, sizeof(request_num)) == -1) {
    LOG_ERROR("请求背包指定道具数量出错,玩家[%lu],道具[%d]", game_actor->GetActorID(), this->require_id_);
    return false;
  }

  if((core::uint32)request_num.number_ < this->count_) {
    return false;  
  }
  return true;
}

void CollectItemCondition::Synchronize() {
/*
  if(this->task_ == NULL) {
    global::LogError("%s:%d (%s) 同步任务数据时 task_ 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
*/
}

void CollectItemCondition::Clear() {
  if(this->task_ == NULL) {
    LOG_ERROR("玩家对象没有找到");
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
  request::RequestRemoveItem remove;
  remove.__set_actor_id_(game_actor->GetActorID());
  remove.__set_item_template_(this->require_id_);
  remove.__set_number_(this->count_);
  remove.__set_bind_(entity::BindRequireType::BOTH);

  if(ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_REMOVE_ITEM,
        &remove, sizeof(remove)) == -1) {
    LOG_ERROR("请求删除背包任务道具出错");
    return ;
  }
}

}  // namespace task

}  // namespace server

}  // namespace game

