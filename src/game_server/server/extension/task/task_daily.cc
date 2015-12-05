
#include "game_server/server/extension/task/task_daily.h"

#include "gateway_server/protocol/gateway_task_protocol_types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/extension/task/configure.h"
#include "game_server/server/extension/task/load_task_daily_configure.h"
#include "game_server/server/extension/task/task_actor.h"
#include "game_server/server/extension/task/task_builder.h"
#include "global/configure/configure.h"

namespace game {

namespace server {

namespace task {

TaskDaily::TaskDaily() {}

TaskDaily::~TaskDaily() {}

bool TaskDaily::Initialize(TaskActor *actor, database::protocol::StorageTaskActorField &data) {
  if(actor == NULL) {
    LOG_ERROR("任务对象没有找到");
    return false;
  }
  this->actor_ = actor;
  this->daily_award_exploit_ = data.daily_exploit_;
  const LoadTaskExploitAwardConfigure::AwardMap &awards = Configure::GetInstance()->GetExploitAwardConfigure();
  this->excploit_awards_.resize(awards.size());

  for(size_t i = 0; i < this->excploit_awards_.size(); ++i) {
    if(i == 0) {
      if((data.exploit_award_ & 1) != 0) {
        this->excploit_awards_[i] = true;
      }
    } else if(i == 1) {
      if((data.exploit_award_ & 2) != 0) {
        this->excploit_awards_[i] = true;
      }
    } else if(i == 2) {
      if((data.exploit_award_ & 4) != 0) {
        this->excploit_awards_[i] = true;
      }
    } else if(i == 3) {
      if((data.exploit_award_ & 8) != 0) {
        this->excploit_awards_[i] = true;
      }
    } else if(i == 4) {
      if((data.exploit_award_ & 16) != 0) {
        this->excploit_awards_[i] = true;
      }
    } else {
      LOG_ERROR("目前只加载到4个");
    }
  }
  GameActor *game_actor = this->actor_->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("玩家对象没有找到");
    return false;
  }
  this->SumAttr(game_actor->GetAttribute(entity::ActorClientFields::HONOUR), true, true);

  return true;
}

void TaskDaily::Finalize() {
  this->actor_ = NULL;
  for(TaskMap::iterator itr = this->tasks_.begin(); itr != this->tasks_.end(); ++itr) {
    if(itr->second == NULL) {
      LOG_ERROR("任务对象数据有问题");
    } else {
      itr->second->Finalize();
      TaskBuilder::GetInstance()->Destory(itr->second);
      itr->second = NULL;
    }
  }
  this->daily_award_exploit_ = 0;
  this->finish_tasks_.clear();
  this->excploit_awards_.clear();
}

void TaskDaily::Save(database::protocol::StorageTaskActorField &field) {
  for(size_t i = 0; i < this->excploit_awards_.size(); ++i) {
    if(this->excploit_awards_[i] == true) {
      switch(i) {
        case 0:
          field.exploit_award_ = field.exploit_award_ | 1;
          break;
        case 1:
          field.exploit_award_ = field.exploit_award_ | 2;
          break;
        case 2:
          field.exploit_award_ = field.exploit_award_ | 4;
          break;
        case 3:
          field.exploit_award_ = field.exploit_award_ | 8;
          break;
        case 4:
          field.exploit_award_ = field.exploit_award_ | 16;
          break;
        default:
          LOG_ERROR("保存暂不支持超过5的");
          break;
      }
    }
  }
  field.__set_daily_exploit_(this->daily_award_exploit_);
}

void TaskDaily::OnExploitEvent(core::int32 num) {
  if(num > 0) {
    // 计算奖励池
    const LoadTaskExploitAwardConfigure::AwardMap &awards = Configure::GetInstance()->GetExploitAwardConfigure();
    core::int32 sum = 0;
    for(LoadTaskExploitAwardConfigure::AwardMap::const_iterator itr = awards.begin();
        itr != awards.end(); ++itr) {
      if(itr->first > this->daily_award_exploit_ && itr->first <= this->daily_award_exploit_ + num &&
          sum - 1 >= 0) {
        this->excploit_awards_[sum-1] = true; 
      }
      sum++;
    }
  }
  this->daily_award_exploit_ += num;
}

void TaskDaily::SaveTask(database::protocol::StorageTaskSaveRequest&  request) {
  for(TaskMap::const_iterator itr = this->tasks_.begin(); itr != this->tasks_.end();
      ++itr) {
    if(itr->second != NULL) {
      database::protocol::StorageTaskField field;
      itr->second->ExportDatabaseField(field);
      request.actives_.push_back(field);
    }
  }

  // 保存完成的任务
  for(TaskHashset::iterator itr1 = this->finish_tasks_.begin(); itr1 != this->finish_tasks_.end();
      ++itr1) {
    database::protocol::StorageTaskFinish task;
    task.__set_type_(entity::TaskType::TASK_DAILY);
    task.__set_task_id_(*itr1);
    request.finishes_.push_back(task);
  }
}

void TaskDaily::Syschronize() {
  gateway::protocol::MessageTaskDailyInitialize message;
  message.__set_exploit_awards_(this->excploit_awards_);
  message.__set_daily_exploit_(this->daily_award_exploit_);
  for(TaskHashset::const_iterator itr = this->finish_tasks_.begin(); itr != this->finish_tasks_.end();
      ++itr) {
    message.finish_tasks_.push_back(*itr);
  }
  for(TaskMap::iterator itr = this->tasks_.begin(); itr != this->tasks_.end(); ++itr) {
    if(itr->second != NULL) {
      gateway::protocol::TaskFields var;
      itr->second->ExportVariable(var);
      message.tasks_.push_back(var);
    }
  }
  if(this->actor_ == NULL) {
    LOG_ERROR("任务对象没有找到");
    return ;
  }

  GameActor *game_actor = this->actor_->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("玩家游戏对象没有找到");
    return ;
  }

  game_actor->SendMessage(message,
    gateway::protocol::MessageType::MESSAGE_TASK_DAILY_INITIALIZE);

}

void TaskDaily::AddTask(Task *task) {
  if(task == NULL) {
    LOG_ERROR("任务指针为空");
    return ;
  }
  this->tasks_.insert(std::make_pair(task->GetID(), task));
}

bool TaskDaily::CheckTaskFinish(core::uint32 task) {
  if(task == 0) {
    return true;
  }
  TaskMap::iterator itr = this->tasks_.find(task);
  if(itr == this->tasks_.end()) {
    return false;
  }
  return itr->second->CheckCondition();
}

bool TaskDaily::CheckTaskAccept(const core::uint32 task) {
  TaskHashset::const_iterator itr_finish = this->finish_tasks_.find((core::int32)task);
  if(itr_finish != this->finish_tasks_.end()) {
    return false;
  }
  TaskMap::const_iterator itr = this->tasks_.find(task);
  if(itr != this->tasks_.end()) {
    return false;
  }
  const TaskDailyCell *cell = Configure::GetInstance()->GetTaskDailyCell((core::int32)task);
  if(cell == NULL) {
    LOG_ERROR("配置(%d)没有找到", task);
    return false;
  }
  itr_finish = this->finish_tasks_.find(cell->front_id_);
  if(itr_finish != finish_tasks_.end() || cell->front_id_ == 0) {
    return true;
  }

  return false;
}

bool TaskDaily::FinishTask(core::int32 id) {
  TaskMap::iterator itr = this->tasks_.find(id);
  if(itr == this->tasks_.end()) {
    return false;
  }
  if(itr->second == NULL) {
    LOG_ERROR("任务列表出错");
    return false;
  }
  // 奖励资源
  const TaskDailyCell *cell = Configure::GetInstance()->GetTaskDailyCell(id);
  if(cell == NULL) {
    LOG_ERROR("配置没有找到(%d)", id);
    return false;
  }
  GameActor *game_actor = this->actor_->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("游戏对象没有找到");
    return false;
  }

  game_actor->AddResource(entity::ResourceID::HONOUR, cell->award_);

  // 删除任务
  itr->second->Clear();
  itr->second->Finalize();
  TaskBuilder::GetInstance()->Destory(itr->second);

  this->tasks_.erase(itr);

  this->finish_tasks_.insert(id);

  this->OnExploitEvent(cell->award_);

  return true;
}

bool TaskDaily::CheckDailyAward(core::int32 id) {
  if(id < (core::int32)this->excploit_awards_.size() && id >= 0) {
    return this->excploit_awards_[id];
  }
  return false;
}


void TaskDaily::SetPicked(core::int32 picked) {
  if(picked >= (core::int32)this->excploit_awards_.size() && picked < 0) {
    return ;
  }
  this->excploit_awards_[picked] = false;
}

void TaskDaily::OnEvent(event::EventType::type type, const void *data, size_t size) {
  for(TaskMap::iterator itr = this->tasks_.begin(); itr != this->tasks_.end(); ++itr) {
    if(itr->second != NULL) {
      itr->second->OnEvent(type, data, size);
    }
  }
}

bool TaskDaily::CheckGather(core::int32 npc_template) {
  for(TaskMap::iterator itr = this->tasks_.begin(); itr != this->tasks_.end(); ++itr) {
    if(itr->second != NULL && itr->second->CheckGather(npc_template) == true) {
      return true;
    }
  }
  return false;
}

bool TaskDaily::CheckSummonNpc(core::int32 npc_template) {
  for(TaskMap::iterator itr = this->tasks_.begin(); itr != this->tasks_.end(); ++itr) {
    if(itr->second != NULL && itr->second->CheckSummonNpc(npc_template) == true) {
      return true;
    }
  }
  return false;
}


void TaskDaily::SumAttr(core::int32 honour, bool add_des, bool sync) {
  // 获取军功
  GameActor *game_actor = this->actor_->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("玩家游戏对象没有找到");
    return ;
  }

  const ExploitAttrCell *cell = Configure::GetInstance()->GetExploitAttrCell(honour);
  if(cell == NULL) {
    return ;
  }
  if(add_des == true) {
    game_actor->SetAttribute(entity::RoleServerFields::PHYSIQUE_ADD_VALUE,
        game_actor->GetAttribute(entity::RoleServerFields::PHYSIQUE_ADD_VALUE) 
        + cell->physique_, sync);
    game_actor->SetAttribute(entity::RoleServerFields::STRENGTH_ADD_VALUE,
        game_actor->GetAttribute(entity::RoleServerFields::STRENGTH_ADD_VALUE)
        + cell->strength_, sync);
    game_actor->SetAttribute(entity::RoleServerFields::INTELLIGENCE_ADD_VALUE,
        game_actor->GetAttribute(entity::RoleServerFields::INTELLIGENCE_ADD_VALUE)
        + cell->intelligence_, sync);
    game_actor->SetAttribute(entity::RoleServerFields::AGILE_ADD_VALUE,
        game_actor->GetAttribute(entity::RoleServerFields::AGILE_ADD_VALUE)
        + cell->agile_, sync);
    game_actor->SetAttribute(entity::RoleServerFields::SPIRIT_ADD_VALUE,
        game_actor->GetAttribute(entity::RoleServerFields::SPIRIT_ADD_VALUE)
        + cell->spirit_, sync);
  } else {
    game_actor->SetAttribute(entity::RoleServerFields::PHYSIQUE_ADD_VALUE,
        game_actor->GetAttribute(entity::RoleServerFields::PHYSIQUE_ADD_VALUE) 
        - cell->physique_, sync);
    game_actor->SetAttribute(entity::RoleServerFields::STRENGTH_ADD_VALUE,
        game_actor->GetAttribute(entity::RoleServerFields::STRENGTH_ADD_VALUE)
        - cell->strength_, sync);
    game_actor->SetAttribute(entity::RoleServerFields::INTELLIGENCE_ADD_VALUE,
        game_actor->GetAttribute(entity::RoleServerFields::INTELLIGENCE_ADD_VALUE)
        - cell->intelligence_, sync);
    game_actor->SetAttribute(entity::RoleServerFields::AGILE_ADD_VALUE,
        game_actor->GetAttribute(entity::RoleServerFields::AGILE_ADD_VALUE)
        - cell->agile_, sync);
    game_actor->SetAttribute(entity::RoleServerFields::SPIRIT_ADD_VALUE,
        game_actor->GetAttribute(entity::RoleServerFields::SPIRIT_ADD_VALUE)
        - cell->spirit_, sync);
  }
}

void TaskDaily::DailyClean(core::int32 days) {
  if(days == 0) {
    return ;
  }
  GameActor *game_actor = this->actor_->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("游戏玩家对象没有找到");
    return ;
  }
  // 定时清理
  for(TaskMap::iterator itr = this->tasks_.begin(); itr != this->tasks_.end(); ++itr) {
    if(itr->second == NULL) {
      LOG_ERROR("任务对象数据有问题");
    } else {
      itr->second->Finalize();
      TaskBuilder::GetInstance()->Destory(itr->second);
      itr->second = NULL;
    }
  }
  this->tasks_.clear();
  this->daily_award_exploit_ = 0;
  this->finish_tasks_.clear();
  this->excploit_awards_.clear();
  this->DailyTaskInit();
  this->Syschronize();
}

void TaskDaily::DailyTaskInit() {
  const LoadTaskDailyConfigure::TaskDailyMap &dailys = Configure::GetInstance()->GetDailyList();
  for(LoadTaskDailyConfigure::TaskDailyMap::const_iterator itr = dailys.begin(); itr != dailys.end();
      ++itr) {
    if(itr->second.front_id_ == 0) {
      Task *task = TaskBuilder::GetInstance()->Create(this->actor_, entity::TaskType::TASK_DAILY, itr->second.id_);
      if(task == NULL) {
        LOG_ERROR("每日任务(%d)加入失败", itr->second.id_);
      } else {
        this->AddTask(task);
      }
      if(task->CheckCondition() == true) {
        this->FinishTask(task->GetID());
      }
    }
  }
}

} // namespace task

} // namespace server

} // namepsace game
