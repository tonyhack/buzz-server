//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-10-25 00:13:52.
// File name: task_trip.cc
//
// Description:
//
#include "game_server/server/extension/task/task_trip.h"

#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_task_protocol_types.h"
#include "game_server/server/extension/task/task_actor.h"
#include "game_server/server/extension/task/task_builder.h"
#include "game_server/server/extension/task/load_trip_configure.h"
#include "game_server/server/extension/task/configure.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace task {

TaskTrip::TaskTrip() {}
TaskTrip::~TaskTrip() {}

bool TaskTrip::Initialize(TaskActor *actor) {
  if(actor == NULL) {
    LOG_ERROR("玩家任务对象没有找到");
    return false;
  }
  this->task_num_ = 0;
  this->actor_ = actor;
  this->tasks_.clear();

  return true;
}

void TaskTrip::Finalize() {
  this->task_num_ = 0;
  this->actor_ = NULL;
  for(TaskMap::iterator itr = this->tasks_.begin(); 
     itr != this->tasks_.end(); ++itr) {
    if(itr->second != NULL) {
      TaskBuilder::GetInstance()->Destory(itr->second);
    }
  }
  this->tasks_.clear();
}

void TaskTrip::Save(database::protocol::StorageTaskActorField &field) {
  field.__set_trip_num_(this->task_num_);
}

void TaskTrip::SaveTask(database::protocol::StorageTaskSaveRequest& request) {
  for(TaskMap::iterator itr = this->tasks_.begin(); itr!=this->tasks_.end(); ++itr) {
     database::protocol::StorageTaskField field;
     field.__set_task_type_(entity::TaskType::TASK_TRIP);
     itr->second->ExportDatabaseField(field);
     request.actives_.push_back(field);
  }
}

bool TaskTrip::AddTask(Task *task) {
  if(task == NULL) {
    LOG_ERROR("玩家任务对象没有找到");
    return false;
  }
  std::pair<TaskMap::iterator,bool> ret;
  ret = this->tasks_.insert(std::make_pair(task->GetID(), task));
  return ret.second;
}

Task *TaskTrip::GetTask(core::uint32 id) {
  TaskMap::iterator iterator = this->tasks_.find(id);
  if(iterator == this->tasks_.end()) {
    return NULL;
  }
  return iterator->second;
}

bool TaskTrip::RemoveTask(core::uint32 id) {
  TaskMap::iterator iterator = this->tasks_.find(id);
  if(iterator == this->tasks_.end()) {
    return false;
  }
  this->tasks_.erase(iterator);
  return true;
}

void TaskTrip::OnEvent(event::EventType::type type, const void *data, size_t size) {
  for(TaskMap::iterator iterator = this->tasks_.begin(); iterator!= this->tasks_.end();
      ++iterator) {
    if(iterator->second != NULL) {
      iterator->second->OnEvent(type, data, size);
    } else {
      LOG_ERROR("奇遇任务出现错误");
    }
  }
}

bool TaskTrip::CheckTaskFinish(core::uint32 task) {
  Task *trip = GetTask(task);
  if(trip == NULL) {
    LOG_ERROR("奇遇任务没有找到");
    return false;
  }
  return trip->CheckCondition();
}

void TaskTrip::TripSynchoronize() {
  if(this->actor_ == NULL) {
    LOG_ERROR("玩家任务对象没有找到");
    return ;
  }
  GameActor *game_actor = this->actor_->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("玩家对象没有找到");
    return ;
  }

  gateway::protocol::MessageTaskTripInitialize init;
  if((core::uint32)this->task_num_ < this->tasks_.size()) {
    LOG_ERROR("奇遇任务数值不对");
    return ;
  }
  init.__set_task_num_(this->task_num_-this->tasks_.size());
  for(TaskMap::iterator itr = this->tasks_.begin(); 
      itr != this->tasks_.end(); ++itr) {
    if(itr->second != NULL) {
      gateway::protocol::TaskFields vars;
      itr->second->ExportVariable(vars);
      init.tasks_.push_back(vars);
    } else {
      LOG_ERROR("奇遇任务出错");
      return ;
    }
  }
  game_actor->SendMessage(init,
      gateway::protocol::MessageType::MESSAGE_TASK_TRIP_INITIALIZE);
}

bool TaskTrip::CheckTaskAccept(const core::uint32 task) {
  //获取玩家等级
  if(this->actor_ == NULL) {
    LOG_ERROR("玩家任务指针为空");
    return false;
  }
  const TripTaskCell *cell_new_id = Configure::GetInstance()->GetTripConditionCell(task);
  if(cell_new_id == NULL) {
    LOG_ERROR("新的任务ID没有找到配置[%d]", task);
    return false;
  }
  TaskMap::const_iterator itr = this->tasks_.begin();
  while(itr != this->tasks_.end()) {
    if(itr->second == NULL) {
      LOG_ERROR("奇遇任务列表有错误指针");
      return false;
    }
    if(itr->second->GetID() == task) {
      return false;
    }
    const TripTaskCell * cell = Configure::GetInstance()->GetTripConditionCell(
        itr->second->GetID());
    if(cell == NULL) {
      LOG_ERROR("配置文件有错误[%d]", itr->second->GetID());
      return false;
    }
    if(cell->type_ == cell_new_id->type_) {
      return false;
    }
    ++itr;
  }
  return true;
}

bool TaskTrip::CheckGather(core::int32 npc_template) {
  for(TaskMap::const_iterator itr = this->tasks_.begin();
      itr != this->tasks_.end(); ++itr) {
    if(itr->second != NULL) {
      if(itr->second->CheckGather(npc_template) == true) {
        return true;
      }
    }
  }
  return false;
}

bool TaskTrip::CheckSummonNpc(core::int32 npc_Template) {
  for(TaskMap::const_iterator itr = this->tasks_.begin();
      itr != this->tasks_.end(); ++itr) {
    if(itr->second) {
      if(itr->second->CheckSummonNpc(npc_Template) == true) {
        return true;
      }
    }
  }
  return false;
}

} // namespace task

} // namespace server

} // namespace game

