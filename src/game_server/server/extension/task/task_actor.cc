//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-12 13:31:37.
// File name: task_actor.cc
//
// Description:
// Define class TaskActor.
//

#include "game_server/server/extension/task/task_actor.h"

#include "database_server/client/database_client.h"
#include "database_server/protocol/storage_task_save_types.h"
#include "database_server/protocol/storage_task_actor_save_types.h"
#include "game_server/server/fight_score_calculator.h"
#include "game_server/server/global_time_tick.h"
#include "game_server/server/extension/task/configure.h"
#include "game_server/server/extension/task/task.h"
#include "game_server/server/extension/task/task_builder.h"
#include "game_server/server/extension/task/task_configure.h"
#include "game_server/server/extension/task/task_circle.h"
#include "game_server/server/extension/task/task_transport.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_task_protocol_types.h"
#include "global/configure/configure.h"

namespace game {

namespace server {

namespace task {

TaskActor::TaskActor() : actor_(NULL) {}
TaskActor::~TaskActor() {}

bool TaskActor::InitializeTasks(const TaskVector &tasks,
      const FinishTaskVector &finishes) {
  gateway::protocol::MessageTaskInitialize message;

  if(this->actor_ == NULL) {
    LOG_ERROR("玩家对象没有找到");
    return false;
  }
  this->save_ = false;

  // 已完成主线任务初始化
  FinishTaskVector::const_iterator iterator1 = finishes.begin();
  for(; iterator1 != finishes.end(); ++iterator1) {
    // 初始化主线任务
    if(iterator1->type_ == entity::TaskType::TASK_MAIN) {
      const TaskCell *cell = 
        Configure::GetInstance()->GetTaskConfigure(iterator1->task_id_);
      if(cell == NULL) {
        LOG_ERROR("玩家(%lu) 初始化完成任务集时找不到任务(%u)配置",
            this->actor_->GetID(), iterator1->task_id_);
        continue;
      }
      this->FinishTask(iterator1->task_id_);
      message.finish_tasks_.push_back(iterator1->task_id_);
      this->AddLayer(iterator1->task_id_, false);
    } else if(iterator1->type_ == entity::TaskType::TASK_DAILY) {
      const TaskDailyCell *cell = Configure::GetInstance()->GetTaskDailyCell(iterator1->task_id_);
      if(cell == NULL) {
        LOG_ERROR("每日任务(%d)已经被删除", iterator1->task_id_);
        continue;
      }
      this->task_daily_.AddFinishTask(iterator1->task_id_);
    }
  }

  // 正在进行任务初始化
  TaskVector::const_iterator iterator2 = tasks.begin();
  for(; iterator2 != tasks.end(); ++iterator2) {
    gateway::protocol::TaskFields vars;
    if(iterator2->flag_ != database::protocol::TaskFlag::ACTIVE) {
      LOG_ERROR("玩家(%lu) 初始化任务时，任务(%u)标志错误",
          this->actor_->GetID(), iterator2->task_);
      return false;
    }
    Task *task = NULL;
    if(iterator2->task_type_ == (core::int8)entity::TaskType::TASK_CIRCLE) {
      if(this->circle_task_.InitializeTask(*iterator2) == false ) {
        return false;
      }
      continue ;
    } else {
      task = TaskBuilder::GetInstance()->Create(this,
          (entity::TaskType::type)iterator2->task_type_, *iterator2);
      if(task == NULL) {
        LOG_ERROR("任务[%d]初始化失败",iterator2->task_);
        return false;
      }
    }
    if(iterator2->task_type_ == (core::int8)entity::TaskType::TASK_MAIN) {
      if(this->AddTask(task) == false) {
        LOG_ERROR("任务[%d]加入失败", iterator2->task_);
        TaskBuilder::GetInstance()->Destory(task);
        return false;
      }
      task->ExportVariable(vars);
      message.tasks_.push_back(vars);
    } else if(iterator2->task_type_ == (core::int8)entity::TaskType::TASK_TRIP) {
      if(this->trip_task_.AddTask(task) == false) {
        LOG_ERROR("任务[%d]加入失败", iterator2->task_);
        TaskBuilder::GetInstance()->Destory(task);
        return false;
      }
    } else if(iterator2->task_type_ == (core::int8)entity::TaskType::TASK_DAILY) {
      this->task_daily_.AddTask(task);
    } else {
      LOG_ERROR("任务类型不存在[%d]",iterator2->task_type_);
      return false;
    }
  }

  if(this->actor_ != NULL && this->actor_->GetLastLogoutTime() == 0) {
    this->task_daily_.DailyTaskInit();
  }
  // 发送主线任务初始化数据到玩家
  this->SendMessage(message, 
      gateway::protocol::MessageType::MESSAGE_TASK_INITIALIZE);
  // 发送循环数据给玩家
  if(this->actor_->CheckFunctionalityState(entity::FunctionalityType::TASK_CIRCLE
        ) == true) {
    this->circle_task_.RoundSynchronize();
  }
  // 发送奇遇任务
  if(this->actor_->CheckFunctionalityState(entity::FunctionalityType::TASK_TRIP
        ) == true) {
    this->trip_task_.TripSynchoronize();
  }
  // 同步每日任务数据
  this->task_daily_.Syschronize();
  this->save_ = true;
  return true;
}

bool TaskActor::Initialize(GameActor *actor, TaskActorFieldResult& task_data) {
  if(actor == NULL) {
    LOG_ERROR("返回错误");
    return false;
  }
  this->actor_ = actor;
  if(this->circle_task_.Initialize(this, task_data) == false) {
    LOG_ERROR("循环任务初始化失败");
    return false;
  }
  this->trip_task_.Initialize(this);
  if(this->task_daily_.Initialize(this, task_data) == false) {
    LOG_ERROR("日常任务初始化失败");
    return false;
  }
  this->trip_task_.SetTaskNum(task_data.trip_num_);

  if (this->task_transport_.Initialize(this, task_data) == false) {
    LOG_ERROR("初始化 task_transport_ 失败");
    return false;
  }

  return true;
}

void TaskActor::Finalize() {
  this->circle_task_.Finalize();
  this->trip_task_.Finalize();
  this->task_daily_.Finalize();
  this->task_transport_.Finalize();
  TaskMap::iterator iterator = this->tasks_.begin();
  for(; iterator != this->tasks_.end(); ++iterator) {
    Task *task = iterator->second;
    if(task == NULL) {
      LOG_ERROR("玩家(%lu) 任务列表中有空对象",
          this->actor_->GetID());
      continue;
    }
    TaskBuilder::GetInstance()->Destory(task);
  }
  this->tasks_.clear();
  this->actor_ = NULL;
  this->save_ = false;
  this->layers_.clear();
  this->finish_tasks_.clear();
}

void TaskActor::Save() {
  if(save_ == false) {
    LOG_INFO("任务扩展没有加载完毕，不保存");
    return ;
  }
  if(this->actor_ == NULL) {
    LOG_ERROR("保存任务失败 actor_ 为 NULL",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  database::protocol::StorageTaskSaveRequest request;
  request.__set_id_(this->GetID());

  // 循环从任务中提取数据到存储消息
  TaskMap::iterator iterator = this->tasks_.begin();
  for(; iterator != this->tasks_.end(); ++iterator) {
    database::protocol::StorageTaskField field;
    Task *task = iterator->second;
    if(task == NULL) {
      LOG_ERROR("玩家(%lu) 任务列表中有空对象",
          this->actor_->GetID());
      continue;
    }
    task->ExportDatabaseField(field);
    request.actives_.push_back(field);
  }

  // 保存循环任务
  this->circle_task_.SaveTask(request);
  
  // 保存奇遇任务
  this->trip_task_.SaveTask(request);

  // 保存完成的主线任务
  TaskHashset::iterator iterator2 = this->finish_tasks_.begin();
  for(; iterator2 != this->finish_tasks_.end(); ++iterator2) {
    database::protocol::StorageTaskFinish task;
    task.__set_type_(entity::TaskType::TASK_MAIN);
    task.__set_task_id_(*iterator2);
    request.finishes_.push_back(task);
  }

  // 保存日常任务
  this->task_daily_.SaveTask(request);

  // 发送消息
  if(database::client::DatabaseClientSingleton::GetInstance().SendRequest(
        request, database::protocol::StorageType::STORAGE_TASK_SAVE,
        this->actor_->GetID()) == false) {
    LOG_ERROR("玩家(%lu) 保存任务信息失败",
        this->actor_->GetID());
    return ;
  }
  
  // 保存任务玩家数据
  database::protocol::StorageTaskActorSaveRequest actor_request;
  database::protocol::StorageTaskActorField actor_field;
  this->circle_task_.Save(actor_field);
  this->trip_task_.Save(actor_field);
  this->task_daily_.Save(actor_field);
  this->task_transport_.Save(actor_field);
  actor_request.__set_data_(actor_field);
  actor_request.__set_actor_id_(this->actor_->GetID());

  // 发送消息
  if(database::client::DatabaseClientSingleton::GetInstance().SendRequest(
        actor_request, database::protocol::StorageType::STORAGE_TASK_ACTOR_SAVE,
        this->actor_->GetID()) == false) {
    LOG_ERROR("玩家(%lu) 保存任务信息失败",
        this->actor_->GetID());
    return ;
  }

  global::LogDebug("玩家(%lu) 保存任务信息成功", this->actor_->GetID());
}

bool TaskActor::AddTask(Task *task) {
  if(task == NULL || this->actor_ == NULL) {
    LOG_ERROR("参数 task或actor_ 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  TaskMap::iterator iterator = this->tasks_.find(task->GetID());
  if(iterator != this->tasks_.end()) {
    LOG_ERROR("玩家(%lu)任务列表中已存在任务(%u)",this->actor_->GetID(),
        task->GetID());
    return false;
  }

  this->tasks_.insert(std::make_pair(task->GetID(), task));

  return true;
}

Task *TaskActor::AddTaskById(core::uint32 id) {
  // 创建一个任务对象
  Task *task = TaskBuilder::GetInstance()->Create(
      this, entity::TaskType::TASK_MAIN, id);
  if(task == NULL) {
    LOG_ERROR("创建任务[%d]对象失败", id);
    return NULL;
  }

  // 增加到任务列表
  if(this->AddTask(task) == false) {
    TaskBuilder::GetInstance()->Destory(task);
    LOG_ERROR("增加任务到玩家任务[%d]列表失败", id);
    return NULL;
  }
  return task;
}

Task *TaskActor::GetTask(core::uint32 id) {
  TaskMap::iterator iterator = this->tasks_.find(id);
  if(iterator != this->tasks_.end()) {

    return iterator->second;
  }

  return NULL;
}

Task *TaskActor::RemoveTask(core::uint32 id) {
  Task *task = NULL;
  TaskMap::iterator iterator = this->tasks_.find(id);
  if(iterator != this->tasks_.end()) {
    task = iterator->second;
    this->tasks_.erase(iterator);
  }

  return task;
}

void TaskActor::OnEvent(event::EventType::type type, 
    const void *data, size_t size) {
  TaskMap::iterator iterator = this->tasks_.begin();
  for(; iterator != this->tasks_.end(); ++iterator) {
    if(iterator->second) {
      iterator->second->OnEvent(type, data, size);
    }
  }
  this->circle_task_.OnEvent(type, data, size);
  this->trip_task_.OnEvent(type, data, size);
  this->task_daily_.OnEvent(type, data, size);
  this->task_transport_.OnEvent(type, data, size);
}

void TaskActor::DailyClean(core::int32 days) {
  if(this->actor_ == NULL) {
    LOG_ERROR("玩家对象为空");
    return ;
  }
  if(this->actor_->CheckFunctionalityState(
        entity::FunctionalityType::TASK_CIRCLE) == true) {
    this->CountCircleData(days);
    this->circle_task_.RoundSynchronize();
  }
  if(this->actor_->CheckFunctionalityState(
        entity::FunctionalityType::TASK_TRIP) == true) {
    this->CountTripData(days);
    this->trip_task_.TripSynchoronize();
  }
  this->task_daily_.DailyClean(days);

  this->task_transport_.DailyClean();
}

void TaskActor::CountCircleData(core::int32 days) {
  if(this->actor_ == NULL) {
    LOG_ERROR("玩家对象为空");
    return ;
  }
  this->circle_task_.InitCircle(days, false);
}

void TaskActor::CountTripData(core::int32 days) {
  if(this->actor_ == NULL) {
    LOG_ERROR("玩家对象为空");
    return ;
  }
  core::int32 remain = this->circle_task_.GetCurrentTotalRound() -
    this->circle_task_.GetCurrentRound();
  this->trip_task_.SetTaskNum(this->trip_task_.GetExecTaskNum());
}

bool TaskActor::CheckGatherTask(core::int32 template_id) {
  // 检查主线任务
  for(TaskMap::const_iterator itr = this->tasks_.begin();
      itr != this->tasks_.end(); ++itr) {
    // 得到配置
    if(itr->second != NULL) {
      if(itr->second->CheckGather(template_id) == true) {
        return true;
      }
    }
  }
  // 检查循环任务
  if(this->circle_task_.CheckGather(template_id) == true) {
    return true;
  }
  // 检查每日任务
  if(this->task_daily_.CheckGather(template_id) == true) {
    return true;
  }
  // 检查奇遇任务
  return this->trip_task_.CheckGather(template_id);
}

void TaskActor::AddLayer(core::uint32 task_id, bool sync) {
  // 获取配置文件
  const TaskLayerCell *cell = Configure::GetInstance()->GetLayerCell(task_id);
  if(cell == NULL) {
    return ;
  }
  // 加入纹章列表
  if(this->layers_.find(cell->layer_) != this->layers_.end()) {
    return ;
  }
  this->layers_.insert(cell->layer_);
  if(this->actor_ == NULL) {
    LOG_ERROR("玩家 GameActor 没有找到");
    return ;
  }
  GameActor *game_actor = this->actor_;
  game_actor->SetAttribute(entity::RoleServerFields::PHYSIQUE_ADD_VALUE,
      game_actor->GetAttribute(entity::RoleServerFields::PHYSIQUE_ADD_VALUE)
      + cell->physique_add_value_, sync);
  game_actor->SetAttribute(entity::RoleServerFields::STRENGTH_ADD_VALUE,
      game_actor->GetAttribute(entity::RoleServerFields::STRENGTH_ADD_VALUE)
      + cell->strength_add_value_, sync);
  game_actor->SetAttribute(entity::RoleServerFields::INTELLIGENCE_ADD_VALUE,
      game_actor->GetAttribute(entity::RoleServerFields::INTELLIGENCE_ADD_VALUE)
      + cell->intelligence_add_value_, sync);
  game_actor->SetAttribute(entity::RoleServerFields::AGILE_ADD_VALUE,
      game_actor->GetAttribute(entity::RoleServerFields::AGILE_ADD_VALUE)
      + cell->agile_add_value_, sync);
  game_actor->SetAttribute(entity::RoleServerFields::SPIRIT_ADD_VALUE,
      game_actor->GetAttribute(entity::RoleServerFields::SPIRIT_ADD_VALUE)
      + cell->spirit_add_value_, sync);
  // 计算战斗力
  FightScoreCalculator calc;
  calc.physique_ = cell->physique_add_value_;
  calc.strength_ = cell->strength_add_value_;
  calc.spirit_ = cell->spirit_add_value_;
  calc.agile_ = cell->agile_add_value_;
  calc.intelligence_ = cell->intelligence_add_value_;

  game_actor->SetSubFightScore(entity::SubFightScoreType::TASK_LAYER,
      calc.GetFightScore(), sync);
}

bool TaskActor::CheckSummonNpc(core::int32 npc_template) {
  for(TaskMap::const_iterator itr = this->tasks_.begin();
      itr != this->tasks_.end(); ++itr) {
    if(itr->second) {
      if(itr->second->CheckSummonNpc(npc_template) == true) {
        return true;
      }
    }
  }
  if(this->circle_task_.CheckSummonNpc(npc_template) == true) {
    return true;
  }
  if(this->task_daily_.CheckSummonNpc(npc_template) == true) {
    return true;
  }
  return this->trip_task_.CheckSummonNpc(npc_template);
}

Task *TaskActor::GetTask(entity::TaskType::type type, core::uint32 id) {
  if(type == entity::TaskType::TASK_MAIN || type == entity::TaskType::TASK_RISK) {
    return this->GetTask(id);
  } else if(type == entity::TaskType::TASK_CIRCLE) {
    return this->circle_task_.GetTask();
  } else if(type == entity::TaskType::TASK_TRIP) {
    return this->trip_task_.GetTask(id);
  }
  return NULL;
}

bool TaskActor::CheckTaskAccept(core::int32 task) {
  // 得到配置信息
  const TaskCell *cell = Configure::GetInstance()->GetTaskConfigure(task);
  if(cell == NULL) {
    global::LogError("%s:%d (%s) 玩家(%lu) 接受了一个不存在的任务ID(%u)",
        __FILE__, __LINE__, __FUNCTION__, this->GetID(), task);
    return false;
  }

  // 检测该任务的前置任务是否已完成
  if(this->CheckTaskFinish(cell->pre_task_id_) == false) {
    global::LogError("%s:%d (%s) 玩家请求接受任务时，该任务的前置任务未完成",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 检测该任务是否已完成
  if(this->CheckTaskFinish(task) == true) {
    global::LogError("%s:%d (%s) 玩家请求接受任务时，该任务已完成",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 任务是否已接受
  if(this->GetTask(task) != NULL) {
    global::LogError("%s:%d (%s) 玩家请求接受任务时，该任务已在任务列表中",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  return true;
}

}  // namespace task

}  // namespace server

}  // namespace game

