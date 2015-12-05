//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-10-09 17:56:45.
// File name: kill_mob_collect_condition.cc
//
// Description:
//

#include "game_server/server/extension/task/kill_mob_collect_condition.h"

#include "game_server/server/game_actor.h"
#include "game_server/server/game_npc.h"
#include "game_server/server/game_npc_manager.h"
#include "game_server/server/extension/task/task.h"
#include "game_server/server/extension/task/task_configure.h"
#include "game_server/server/extension/task/configure.h"
#include "game_server/server/extension/task/task_actor.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/request/game_item_request_types.h"
#include "gateway_server/protocol/gateway_task_protocol_types.h"
#include "global/logging.h"
#include "global/types.h"

namespace game {

namespace server {

namespace task {

KillMobCollectCondition::KillMobCollectCondition() {}
KillMobCollectCondition::~KillMobCollectCondition() {}

bool KillMobCollectCondition::Initialize(Task *task, core::uint32 param1,
    core::uint32 param2, core::uint32 value) {
  if(Condition::Initialize(task, param1, param2, value) == false) {
    global::LogError("%s:%d (%s) 初始化 KillMobCollectCondition 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->require_id_ = param1;
  this->count_ = param2;

  return true;
}

void KillMobCollectCondition::Finalize() {
  Condition::Finalize();
}

bool KillMobCollectCondition::ExportConditionVar(
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
  if(ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_GET_PAKCET_ITEM_NUMBER,
        &request_num, sizeof(request_num)) == -1) {
    LOG_ERROR("请求背包指定道具数量出错,玩家[%lu],道具[%d]", 
        game_actor->GetActorID(), this->require_id_);
    return false;
  }
  var = this->require_id_;
  value = request_num.number_;

  return true;
}

bool KillMobCollectCondition::CheckCondition() {
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

void KillMobCollectCondition::Synchronize() {
  if(this->task_ == NULL) {
    global::LogError("%s:%d (%s) 同步任务数据时 task_ 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
}

void KillMobCollectCondition::OnEvent(event::EventType::type type,
    const void *data, size_t size) {
  if(data == NULL) {
    global::LogError("%s:%d (%s) 参数 data 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  switch(type) {
    case event::EventType::EVENT_ACTOR_KILL_NPC:
      this->OnEventKillMobCollect(data, size);
      break;
    default:
      break;
  }
}

void KillMobCollectCondition::OnEventKillMobCollect(const void *data, size_t size) {
  if(data == NULL || size != sizeof(event::EventActorKillNpc)) {
    global::LogError("%s:%d (%s) 参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  
  event::EventActorKillNpc *event = (event::EventActorKillNpc *)data;

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

  // 如果道具已经足够，就不需要再往背包中添加道具
  request::RequestGetPacketItemNumber request_num;
  request_num.__set_actor_id_(game_actor->GetActorID());
  request_num.__set_item_template_(this->require_id_);
  request_num.__set_bind_(entity::BindRequireType::BOTH);
  if(ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_GET_PAKCET_ITEM_NUMBER,
        &request_num, sizeof(request_num)) == -1) {
    LOG_ERROR("请求背包指定道具数量出错,玩家[%lu],道具[%d]", game_actor->GetActorID(), this->require_id_);
    return ;
  }

  // 随机概率
  const KillCollectCell *kill_cell = Configure::GetInstance()->GetKillCollect(this->require_id_);
  if(kill_cell == NULL) {
    LOG_ERROR("当前的任务ID没有找到");
    return ;
  }
  GameNpc *npc = GameNpcManager::GetInstance()->Get(event->npc_);
  if(npc == NULL) {
    LOG_ERROR("获取 GameNpc(%lu) 失败", event->npc_);
    return ;
  }
  if(kill_cell->npc_ != npc->GetTemplateID()) {
    return ;
  }

  core::uint32 rand_num =
    ExtensionManager::GetInstance()->GetRandom().Random(global::kPercentDenom);
  if(rand_num > kill_cell->odds_) {
    return ;
  }

  if((core::uint32)request_num.number_ >= this->count_) {
    return ;  
  }

  // 添加道具到背包
  request::RequestAddItem add_item;
  add_item.__set_actor_id_(game_actor->GetActorID());
  add_item.__set_item_template_(this->require_id_);
  add_item.__set_number_(1);
  add_item.__set_bind_(entity::BindRequireType::BIND);
  add_item.__set_simulate_(false);

  if(ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_ADD_ITEM,
        &add_item, sizeof(add_item)) == -1) {
    LOG_ERROR("请求背包添加道具出错,玩家[%lu],道具[%d]", game_actor->GetActorID(), this->require_id_);
    return ;
  }
  if(add_item.result_ != request::AddItemResult::SUCCESS) {
    LOG_ERROR("请求背包添加道具出错,玩家[%lu],道具[%d]", game_actor->GetActorID(), this->require_id_);
    return ;
  }

  // 同步任务数据
  // this->Synchronize();
}

void KillMobCollectCondition::Clear() {
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

