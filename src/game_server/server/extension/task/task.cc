//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-10 13:57:31.
// File name: task.cc
//
// Description:
// Define class Task.
//

#include "game_server/server/extension/task/task.h"

#include "entity/task_types.h"
#include "game_server/server/extension/task/condition.h"
#include "game_server/server/extension/task/gather_condition.h"
#include "game_server/server/extension/task/condition_pool.h"
#include "game_server/server/extension/task/configure.h"
#include "game_server/server/extension/task/summon_npc_condition.h"
#include "game_server/server/extension/task/task_actor.h"
#include "game_server/server/extension/task/task_configure.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace task {

Task::Task() {}
Task::~Task() {}

bool Task::Initialize(TaskActor *actor, core::uint32 id, entity::TaskType::type type,
    const database::protocol::StorageTaskField &field) {
  if(actor == NULL) {
    LOG_ERROR("类型[%d]任务[%d]初始化失败", type, id);
    return false;
  }
  this->actor_ = actor;
  this->type_ = type;

  if(entity::TaskType::TASK_CIRCLE == type) {
    const ConditionCell *cell = Configure::GetInstance()->GetConditionCell(id);
    if(cell == NULL) {
      LOG_ERROR("循环任务初始化失败[%d]", id);
      return false;
    }
    this->id_ = id;
    return this->AddCondition(cell->condition_, field);
  } else if(entity::TaskType::TASK_TRIP == type) {
    // 奇遇任务
    const TripTaskCell *cell = Configure::GetInstance()->GetTripConditionCell(id);
    if(cell == NULL) {
      LOG_ERROR("循环任务初始化失败[%d]", id);
      return false;
    }
    this->id_ = id;
    return this->AddCondition(cell->condition_, field);
  } else if(entity::TaskType::TASK_MAIN == type ||
      entity::TaskType::TASK_RISK == type) {
    // 任务配置
    const TaskCell *cell = Configure::GetInstance()->GetTaskConfigure(id);
    if(cell == NULL) {
      global::LogError("%s:%d (%s) 初始化任务(%u)时，找不到相应配置",
          __FILE__, __LINE__, __FUNCTION__, id);
      return false;
    }
    // 分配、加入条件对象
    for(size_t pos = 0; pos < 3; ++pos) {
      if(this->AddCondition(cell->condition_[pos], field) == false) {
        LOG_ERROR("初始化主线任务出错[%d]", id);
        return false;
      }
    }
    this->id_ = id;
  } else if(entity::TaskType::TASK_DAILY == type) {
    const TaskDailyCell *cell = Configure::GetInstance()->GetTaskDailyCell(id);
    if(cell == NULL) {
      LOG_ERROR("每日任务(%d)不存在", id);
      return false;
    }
    this->id_ = id;
    return this->AddCondition(cell->condition_, field);
  }
  return true;
}

void Task::Finalize() {
  ConditionVector::iterator iterator = this->conditions_.begin();
  for(; iterator != this->conditions_.end(); ++iterator) {
    if((*iterator)) {
      (*iterator)->Finalize();
      ConditionPool::GetInstance()->Deallocate((*iterator));
    }
  }

  this->conditions_.clear();

  this->id_ = 0;
  this->actor_ = NULL;
}

void Task::ExportDatabaseField(database::protocol::StorageTaskField &field) {
  field.__set_task_(this->GetID());

  // 第一个条件
  field.__set_task_type_(this->type_);
  ConditionVector::iterator iterator = this->conditions_.begin();
  if(iterator == this->conditions_.end()) {
    return ;
  }
  Condition *condition = *iterator;
  if(condition) {
    field.__set_cond_type1_(condition->GetType());
    condition->ExportConditionVar((core::uint32 &)field.var1_,
        (core::uint32 &)field.value1_);
  }

  // 第二个条件
  ++iterator;
  if(iterator == this->conditions_.end()) {
    return ;
  }
  condition = *iterator;
  if(condition) {
    field.__set_cond_type2_(condition->GetType());
    condition->ExportConditionVar((core::uint32 &)field.var2_,
        (core::uint32 &)field.value2_);
  }

  // 第三个条件
  ++iterator;
  if(iterator == this->conditions_.end()) {
    return ;
  }
  condition = *iterator;
  if(condition) {
    field.__set_cond_type3_(condition->GetType());
    condition->ExportConditionVar((core::uint32 &)field.var3_,
        (core::uint32 &)field.value3_);
  }
}

void Task::ExportVariable(gateway::protocol::TaskFields &var) {
  var.__set_id_(this->GetID());

  gateway::protocol::TaskVar cond_var;

  // 第一个条件
  ConditionVector::iterator iterator = this->conditions_.begin();
  if(iterator == this->conditions_.end()) {
    return ;
  }
  Condition *condition = *iterator;
  if(condition) {
    cond_var.__set_type_(condition->GetType());
    condition->ExportConditionVar((core::uint32 &)cond_var.id_,
        (core::uint32 &)cond_var.value_);
    var.variables_.push_back(cond_var);
  }

  // 第二个条件
  ++iterator;
  if(iterator == this->conditions_.end()) {
    return ;
  }
  condition = *iterator;
  if(condition) {
    cond_var.__set_type_(condition->GetType());
    condition->ExportConditionVar((core::uint32 &)cond_var.id_,
        (core::uint32 &)cond_var.value_);
    var.variables_.push_back(cond_var);
  }

  // 第三个条件
  ++iterator;
  if(iterator == this->conditions_.end()) {
    return ;
  }
  condition = *iterator;
  if(condition) {
    cond_var.__set_type_(condition->GetType());
    condition->ExportConditionVar((core::uint32 &)cond_var.id_,
        (core::uint32 &)cond_var.value_);
    var.variables_.push_back(cond_var);
  }
}

bool Task::AddCondition(Condition *condition) {
  if(condition == NULL) {
    global::LogError("%s:%d (%s) 增加任务条件失败，参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->conditions_.push_back(condition);
  return true;
}

bool Task::CheckCondition() {
  ConditionVector::iterator iterator = this->conditions_.begin();
  for(; iterator != this->conditions_.end(); ++iterator) {
    if((*iterator) && (*iterator)->CheckCondition() == false) {
      return false;
    }
  }

  return true;
}

void Task::OnEvent(event::EventType::type type, const void *data, size_t size) {
  // 遍历所有任务条件，把事件通知到所有条件
  ConditionVector::iterator iterator = this->conditions_.begin();
  for(; iterator != this->conditions_.end(); ++iterator) {
    if((*iterator)) {
      (*iterator)->OnEvent(type, data, size);
    }
  }
}

void Task::SynchronizeVariable(const gateway::protocol::TaskVar &var) {
  if(this->actor_ == NULL) {
    global::LogError("%s:%d (%s) 同步任务变量时，actor_ 对象为空",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  
  // 同步消息
  gateway::protocol::MessageTaskSynchronize message;
  if(this->type_ == entity::TaskType::TASK_CIRCLE) {
    message.task_.__set_id_(0);
  } else {
    message.task_.__set_id_(this->GetID());
  }
  message.task_.variables_.push_back(var);
  message.__set_task_type_(this->type_);
  // 发送到玩家
  this->actor_->SendMessage(message,
      gateway::protocol::MessageType::MESSAGE_TASK_SYNCHRONIZE);
}

void Task::Clear() {
  // 遍历所有任务条件，把事件通知到所有条件
  ConditionVector::iterator iterator = this->conditions_.begin();
  for(; iterator != this->conditions_.end(); ++iterator) {
    if((*iterator)) {
      (*iterator)->Clear();
    }
  }
}

void Task::OnFinish() {
  ConditionVector::iterator iterator = this->conditions_.begin();
  for(; iterator != this->conditions_.end(); ++iterator) {
    if((*iterator)) {
      (*iterator)->OnFinish();
    }
  }
}

void Task::OnAbandon() {
  ConditionVector::iterator iterator = this->conditions_.begin();
  for(; iterator != this->conditions_.end(); ++iterator) {
    if((*iterator)) {
      (*iterator)->OnAbandon();
    }
  }
}

bool Task::AddCondition(const TaskCondition &cell, 
    const database::protocol::StorageTaskField &field) {
  core::uint32 param1 = 0, param2 = 0, value = 0;
  if(cell.type_ > entity::TaskCondition::MIN &&
      cell.type_ < entity::TaskCondition::MAX) {
    Condition *condition = ConditionPool::GetInstance()->Allocate(
        (entity::TaskCondition::type)cell.type_);
    if(condition == NULL) {
      LOG_ERROR("初始化任务失败，分配任务内存池失败(type)", cell.type_);
      return false;
    }

    // 选出其中一种匹配的条件
    if(cell.type_ == (core::uint32)field.cond_type1_ &&
        cell.param1_ == field.var1_) {
      value = field.value1_;
    } else if(cell.type_ == (core::uint32)field.cond_type2_ &&
        cell.param1_ == field.var2_) {
      value = field.value2_;
    } else if(cell.type_ == (core::uint32)field.cond_type3_ &&
        cell.param1_ == field.var3_) {
      value = field.value3_;
    } else {
      value = 0;
    }
    param1 = cell.param1_;
    param2 = cell.param2_;
    // 初始化条件对象
    if(condition->Initialize(this, param1, param2, value) == false) {
      ConditionPool::GetInstance()->Deallocate(condition);
      global::LogError("%s:%d (%s) 初始化任务(%u)条件失败",
          __FILE__, __LINE__, __FUNCTION__, this->id_);
      return false;
    }
    // 加入条件列表
    if(this->AddCondition(condition) == false) {
      condition->Finalize();
      ConditionPool::GetInstance()->Deallocate(condition);
      global::LogError("%s:%d (%s) 初始化任务(%u)时，加入条件对象(%u)失败",
          __FILE__, __LINE__, __FUNCTION__, this->id_, cell.type_);
      return false;
    }
    return true;
  }
  return true;
}

bool Task::CheckGather(core::int32 npc_template) {
  for(core::uint32 i = 0 ; i < this->conditions_.size(); ++i) {
    if(this->conditions_[i] != NULL) {
      if(this->conditions_[i]->GetType() == entity::TaskCondition::PICK) {
        GatherCondition *condition = (GatherCondition *)this->conditions_[i];
        const KillCollectCell *kill_cell = 
          Configure::GetInstance()->GetKillCollect(condition->GetTemplateID());
        if(kill_cell == NULL) {
          LOG_ERROR("配置文件出错[%d]", condition->GetTemplateID());
          return false;
        }
        if((core::int32)kill_cell->npc_ == npc_template) {
          return true;
        }
        return false;
      }
    }
  }
  return true;
}

bool Task::CheckSummonNpc(core::int32 npc_template) {
  ConditionVector::iterator iterator = this->conditions_.begin();
  for(; iterator != this->conditions_.end(); ++iterator) {
    if((*iterator) && (*iterator)->GetType() == 
        entity::TaskCondition::SUMMON_NPC) {
      SummonNpcCondition *condition = (SummonNpcCondition *)(*iterator);
      if(condition->GetTemplate() == npc_template && 
          condition->CheckCondition() == false) {
        return true;
      }
    }
  }
  return false;
}

bool Task::GetCoordinate(Task::TaskCoordinateType type, core::int32 pos, 
    core::int32 &out_map, core::int32 &out_x, core::int32 &out_y) {
  if(entity::TaskType::TASK_CIRCLE == this->type_) {
    const ConditionCell *cell = Configure::GetInstance()->GetConditionCell(this->id_);
    if(cell == NULL) {
      LOG_ERROR("获取循环任务[%d]坐标失败", this->id_);
      return false;
    }
    out_map = cell->coordinate_.map_;
    out_x = cell->coordinate_.x_;
    out_y = cell->coordinate_.y_;
    return true;
  } else if(entity::TaskType::TASK_TRIP == this->type_) {
    const TripTaskCell *cell = Configure::GetInstance()->GetTripConditionCell(this->id_);
    if(cell == NULL) {
      LOG_ERROR("获取历险任务[%d]坐标失败", this->id_);
      return false;
    }
    out_map = cell->coordinate_.map_;
    out_x = cell->coordinate_.x_;
    out_y = cell->coordinate_.y_;
    return true;
  } else if(entity::TaskType::TASK_MAIN == this->type_ ||
      entity::TaskType::TASK_RISK == this->type_) {
    const TaskCell *cell = Configure::GetInstance()->GetTaskConfigure(this->id_);
    if(cell == NULL) {
      LOG_ERROR("获取主线或者奇遇任务[%d][%d]坐标失败", this->id_, pos);
      return false;
    }
    if(type == Task::CONDITION) {
      if(cell->condition_coordinates_.size() < (size_t)pos || (size_t)pos <= 0) {
        LOG_ERROR("获取主线或者奇遇任务[%d][%d]坐标失败", this->id_, pos);
        return false;
      }
      out_map = cell->condition_coordinates_[pos-1].map_;
      out_x = cell->condition_coordinates_[pos-1].x_;
      out_y = cell->condition_coordinates_[pos-1].y_;
      return true;
    } else if(type == Task::ACCEPT) {
      out_map = cell->accept_coordinate_.map_;
      out_x = cell->accept_coordinate_.x_;
      out_y = cell->accept_coordinate_.y_;
    } else if(type == Task::FINISH) {
      out_map = cell->finish_coordinate_.map_;
      out_x = cell->finish_coordinate_.x_;
      out_y = cell->finish_coordinate_.y_;
    } else {
      return false;
    }
    return true;
  } else if(entity::TaskType::TASK_DAILY == this->type_) {
    return false;
  }
  return false;
}

}  // namespace task

}  // namespace server

}  // namespace game

