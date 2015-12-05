
#include "game_server/server/extension/welfare/task.h"

#include "game_server/server/extension/welfare/achieve_actor.h"
#include "game_server/server/extension/welfare/condition.h"
#include "game_server/server/extension/welfare/configure.h"
#include "game_server/server/extension/welfare/condition_pool.h"
#include "game_server/server/extension/welfare/load_task_configure.h"
#include "game_server/server/global_time_tick.h"
#include "global/common_functions.h"

namespace game {

namespace server {

namespace welfare {

Task::Task() {}

Task::~Task() {}

bool Task::Initialize(AchieveActor *actor, core::int32 id, database::protocol::StorageAchieveField &field) {
  if(actor == NULL) {
    LOG_ERROR("成就任务(%d)初始化失败", id);
    return false;
  }
  this->actor_ = actor;
  this->id_ = id;
  this->finish_time_ = field.finish_time_;
  const AchieveTaskCell *cell = Configure::GetInstance()->GetAchieveCell(id);
  if(cell == NULL) {
    LOG_ERROR("成就(%d)不存在", id);
    return false;
  }
  for(size_t i = 0; i < cell->conditions_.size(); ++i) {
    Condition *condition = ConditionPool::GetInstance()->Allocate();
    if(condition == NULL) {
      LOG_ERROR("申请内存失败");
      return false;
    }
    if(i == 0) {
      condition->Initialize(this, (entity::AchieveConditionType::type)cell->conditions_[i].type_, cell->conditions_[i].param1_, cell->conditions_[i].param2_, 
          cell->conditions_[i].param3_, field.condition1_value_);
      this->actor_->AddEventDispatch(id, (entity::AchieveConditionType::type)cell->conditions_[i].type_);
    } else if(i == 1) {
      condition->Initialize(this, (entity::AchieveConditionType::type)cell->conditions_[i].type_, cell->conditions_[i].param1_, cell->conditions_[i].param2_, 
          cell->conditions_[i].param3_, field.condition2_value_);
      this->actor_->AddEventDispatch(id, (entity::AchieveConditionType::type)cell->conditions_[i].type_);
    } else {
      LOG_ERROR("条件太多");
      return false;
    }
    this->conditions_.push_back(condition);
  }
  return true;
}

void Task::Finalize() {
  for(size_t i = 0; i < this->conditions_.size(); ++i) {
    if(this->conditions_[i] != NULL) {
      ConditionPool::GetInstance()->Deallocate(this->conditions_[i]);
    }
  }
  this->conditions_.clear();
  this->actor_ = NULL;
}

void Task::Clear() {
  for(size_t i = 0; i < this->conditions_.size(); ++i) {
    if(this->conditions_[i] != NULL) {
      this->conditions_[i]->Clear();
    }
  }
}

bool Task::CheckFinish() {
  for(size_t i = 0; i < this->conditions_.size(); ++i) {
    if(this->conditions_[i] != NULL) {
      if(this->conditions_[i]->CheckCondition() == false) {
        return false;
      }
    }
  }
  return true;
}

void Task::OnEvent(event::EventType::type type, const void *data, size_t size) {
  for(size_t i = 0; i < this->conditions_.size(); ++i) {
    if(this->conditions_[i] != NULL) {
      this->conditions_[i]->OnEvent(type, data, size);
    }
  }
}

void Task::ExportConditionVar(gateway::protocol::AchieveCondition &condition) {
  for(size_t i = 0; i < this->conditions_.size(); ++i) {
    condition.values_.push_back(this->conditions_[i]->GetValue());
  }
  condition.__set_id_(this->id_);
  condition.__set_finish_time_(global::ToString(this->finish_time_));
}

void Task::Save(database::protocol::StorageAchieveField &field) {
  // 最多导出俩个条件
  field.__set_achieve_id_(this->id_);
  field.__set_finish_time_(this->finish_time_);
  core::int32 pos = 0;
  for(size_t i = 0; i < this->conditions_.size(); ++i) {
    pos += 1;
    if(this->conditions_[i] != NULL && pos == 1) {
      this->conditions_[i]->ExportConditionVar(
          field.condition1_param1_,field.condition1_param2_, field.condition1_value_);
    } else if(this->conditions_[i] != NULL && pos == 2) {
      this->conditions_[i]->ExportConditionVar(
          field.condition2_param1_,field.condition2_param2_, field.condition2_value_);
    } else {
      LOG_ERROR("任务条件出错");
      return ;
    }
  }
  return ;
}

AchieveActor *Task::GetActor() {
  return this->actor_;
}

void Task::Synchronize() {
  AchieveActor *actor = this->actor_;
  if(actor == NULL) {
    LOG_ERROR("玩家成就对象没有找到");
    return ;
  }

  GameActor *game_actor = actor->GetActor();

  if(game_actor == NULL) {
    LOG_ERROR("玩家对象没有找到");
    return ;
  }

  if(this->CheckFinish() == true) {
    this->finish_time_ = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
  }

  gateway::protocol::MessageAchieveConditionSysnchronization message;
  gateway::protocol::AchieveCondition data;
  this->ExportConditionVar(data);
  message.__set_condition_(data);
  game_actor->SendMessage(message,
      gateway::protocol::MessageType::MESSAGE_ACHIEVE_CONDITION_SYSNCHRONIZATION);
}

}  // namespace welfare

}  // namespace server

}  // namespace game
