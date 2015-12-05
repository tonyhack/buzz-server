//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-10-15 19:22:48.
// File name: task_circle.cc
//
// Description:
//
#include "game_server/server/extension/task/task_circle.h"

#include "entity/constants_constants.h"
#include "game_server/server/extension/task/configure.h"
#include "game_server/server/extension/task/load_circle_condition_configure.h"
#include "game_server/server/extension/task/task_actor.h"
#include "game_server/server/extension/task/task_builder.h"
#include "global/logging.h"

namespace game { 

namespace server {

namespace task {

TaskCircle::TaskCircle() {}
TaskCircle::~TaskCircle() {}

bool TaskCircle::Initialize(TaskActor *actor, TaskActorFieldResult& taskData) {
  // 初始化循环数据
  if(actor  == NULL) {
    LOG_ERROR("玩家任务对象没有找到");
    return false;
  }
  this->actor_ = actor;
  this->task_ = NULL;
  this->current_lv_stage_ = taskData.lv_stage_;
  this->current_circle_ = taskData.circle_num_;
  this->flag_ = (entity::TaskCircleRoundState::type)taskData.flag_;
  this->current_round_ = taskData.round_num_;
  this->total_round_ = taskData.total_num_;
  this->current_condition_id_ = taskData.condition_id_;
  return true;
}

bool TaskCircle::InitializeTask(const database::protocol::StorageTaskField &field) {
  if(this->actor_ == NULL) {
    return false;
  }
  if(task_ != NULL) {
    this->task_->Finalize();
    TaskBuilder::GetInstance()->Destory(this->task_);
    this->task_ = NULL;
  }
  if(this->current_condition_id_ == 0) {
    if(this->flag_ == entity::TaskCircleRoundState::FINISH || 
        this->flag_ == entity::TaskCircleRoundState::ACTIVE) {
      LOG_ERROR("循环任务出错");
      this->CloseRound();
    }
    return true;
  }
  this->task_ = TaskBuilder::GetInstance()->Create(actor_, 
      (entity::TaskType::type)field.task_type_, field);
  if(this->task_ == NULL) {
    LOG_ERROR("循环任务初始化失败(%d)", this->current_condition_id_);
    this->CloseRound();
  }
  return true;
}

void TaskCircle::Finalize() {
  if(this->task_ != NULL) {
    TaskBuilder::GetInstance()->Destory(this->task_);
    this->task_ = NULL;
  }
  this->task_ = NULL;
  this->actor_ = NULL;
  this->current_circle_ = 0;
  this->current_round_ = 0;
  this->total_round_ = 0;
  this->current_lv_stage_ = 0;
  this->flag_ = entity::TaskCircleRoundState::PICKED_AWARD;
}

void TaskCircle::Save(database::protocol::StorageTaskActorField &field) {
  field.__set_circle_num_(this->current_circle_);
  field.__set_condition_id_(this->current_condition_id_);
  field.__set_flag_(this->flag_);
  field.__set_lv_stage_(this->current_lv_stage_);
  field.__set_round_num_(this->current_round_);
  field.__set_total_num_(this->total_round_);
}

void TaskCircle::SaveTask(database::protocol::StorageTaskSaveRequest &request) {
  if(this->task_ != NULL) {
    database::protocol::StorageTaskField field;
    this->task_->ExportDatabaseField(field);
    request.actives_.push_back(field);
    LOG_ERROR("循环任务类型[%d]", request.actives_[0].task_type_);
  }
}

bool TaskCircle::CheckCircleFinish() const{
  if(this->task_ != NULL) {
     return this->task_->CheckCondition();
  }
  LOG_ERROR("任务指针没有找到");
  return false;
}

bool TaskCircle::CheckRoundFinish() const {
  return this->flag_ == entity::TaskCircleRoundState::FINISH ? true : false;
}

core::int32 TaskCircle::GetCurrentRound() const {
  return this->current_round_;
}

core::int32 TaskCircle::GetCurrentTotalRound() const {
  return this->total_round_;
}

void TaskCircle::RoundSynchronize() {
  if(this->actor_ == NULL) {
    LOG_ERROR("玩家指针为空");
    return ;
  }
  GameActor *game_actor = this->actor_->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("玩家指针为空");
    return ;
  }

  gateway::protocol::MessageTaskCircleInitialize message;
  message.__set_current_circle_(this->current_circle_);
  message.__set_current_round_(this->current_round_);
  message.__set_total_round_(this->total_round_);
  message.__set_level_stage_(this->current_lv_stage_);
  message.__set_circle_condition_id_(this->current_condition_id_);
  message.__set_state_((entity::TaskCircleRoundState::type)this->flag_);
  gateway::protocol::TaskFields vars;
  if(this->task_ != NULL) {
    this->task_->ExportVariable(vars);
  }
  message.__set_task_(vars);
  game_actor->SendMessage(message, 
      gateway::protocol::MessageType::MESSAGE_TASK_CIRCLE_INITIALIZE);

}

void TaskCircle::SetLvStage() {
  if(this->actor_ == NULL) {
    LOG_ERROR("玩家指针为空");
    return ;
  }
  GameActor *game_actor = this->actor_->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("玩家指针为空");
    return ;
  }
  if(game_actor->CheckFunctionalityState(
        entity::FunctionalityType::TASK_CIRCLE) == false) {
    LOG_ERROR("当前玩家[%lu]的循环任务模块没有打开",
        game_actor->GetActorID());
    return ;
  }
  if(game_actor->GetAttribute(entity::RoleAoiFields::LEVEL) < 
      entity::g_constants_constants.kMaxCircleLimitLevel) {
    LOG_ERROR("当前玩家[%lu]的等级[%d]不够", game_actor->GetActorID(),
        game_actor->GetAttribute(entity::RoleAoiFields::LEVEL));
    return ;
  }
  this->current_lv_stage_ = 
    game_actor->GetAttribute(entity::RoleAoiFields::LEVEL)/
    entity::g_constants_constants.kMaxCirclelevelStage -
    entity::g_constants_constants.kMaxCircleLevelStageStart;
}

bool TaskCircle::OpenRound() {
  if(this->current_round_ >= this->total_round_) {
    LOG_ERROR("循环任务[%d]全部已经做完[%d]", this->current_round_, 
        this->total_round_);
    return false;
  }
  if(this->flag_ != entity::TaskCircleRoundState::INVALID) {
    LOG_ERROR("循环任务状态(%d)错误", flag_);
    return false;
  }
  this->flag_ = entity::TaskCircleRoundState::ACTIVE;
  // 设置等级段
  this->SetLvStage();
  // 随机任务对象
  core::int32 condition_id = this->RandConditon();
  if(condition_id == -1) {
    LOG_ERROR("随机条件出错,等级段(%d)", this->current_lv_stage_);
    return false;
  }
  if(this->CreateTask(condition_id) == NULL) {
    LOG_ERROR("创建任务(%d)失败", condition_id);
    return false;
  }
  this->current_round_ += 1;
  this->current_circle_ = 1;
  return true;
}

bool TaskCircle::CloseTaskCircle() {
  if(this->current_round_ < this->total_round_) {
    LOG_ERROR("无法关闭，当前的轮数目(%d)没有达到最大可做的(%d)",
        this->current_round_, this->total_round_);
    return false;
  }
  this->flag_ = entity::TaskCircleRoundState::PICKED_AWARD;
  return true;
}

bool TaskCircle::OpenTaskCircle() {
  if(this->current_round_ >= this->total_round_) {
    LOG_ERROR("无法开启");
    return false;
  }
  this->flag_ = entity::TaskCircleRoundState::INVALID;
  return true;
}

void TaskCircle::CloseRound() {
  this->current_circle_ = 0;
  if(this->task_ != NULL) {
    this->task_->Clear();
    this->task_->Finalize();
    TaskBuilder::GetInstance()->Destory(this->task_);
    this->task_ = NULL;
  }
  this->current_condition_id_ = 0;
  this->current_circle_ = 0;
  this->flag_ = entity::TaskCircleRoundState::INVALID;
}

core::int32 TaskCircle::RandConditon() {
  const ConditionCell * condition_cell = 
    Configure::GetInstance()->RandConditionCell(this->current_lv_stage_);
  if(condition_cell == NULL) {
    LOG_ERROR("循环任务随机出错,等级段(%d)", this->current_lv_stage_);
    return -1;
  }

  return condition_cell->order_id_;
}

bool TaskCircle::ReplaceTask() {
  if(this->CreateTask(this->RandConditon()) == NULL) {
    return false;
  }
  return true;
}

Task *TaskCircle::CreateTask(core::int32 conditon) {
  if(this->flag_ != entity::TaskCircleRoundState::ACTIVE) {
    LOG_ERROR("循环任务非活动状态下(%d)不能随机任务",
        this->flag_);
    return NULL;
  }
  Task *task = TaskBuilder::GetInstance()->Create(
      this->actor_, entity::TaskType::TASK_CIRCLE, conditon);
  if(task == NULL) {
    LOG_ERROR("创建循环任务失败(%d)", conditon);
    return NULL;
  }
  if(this->task_ !=NULL) {
    this->task_->Clear();
    this->task_->Finalize();
    TaskBuilder::GetInstance()->Destory(this->task_);
    this->task_ = NULL;
  }
  this->task_ = task;
  this->current_condition_id_ = conditon;
  return this->task_;
}

bool TaskCircle::NextCircle() {
  if(this->flag_ != entity::TaskCircleRoundState::ACTIVE) {
    LOG_ERROR("当前的状态(%d)不能进行下一个循环任务", this->flag_);
    return false;
  }
  if(this->current_circle_ >= 20) {
    LOG_ERROR("当前轮的循环任务已经全部完成");
    this->flag_ = entity::TaskCircleRoundState::FINISH;
    return false;
  }
  if(this->ReplaceTask() == false) {
    return false;
  }
  this->current_circle_ += 1;
  return true;
}

void TaskCircle::InitCircle(core::int32 num, bool new_role) {
  if(new_role == true) {
    this->total_round_ = 1;
    this->current_round_ = 0;
    this->flag_ = entity::TaskCircleRoundState::INVALID;
    this->current_condition_id_ = 0;
    this->current_circle_ = 0;
    if(this->task_ != NULL) {
      this->task_->Clear();
      this->task_->Finalize();
      TaskBuilder::GetInstance()->Destory(this->task_);
    }
    this->task_ = NULL;
  } else {
    core::int32 old = this->total_round_ - this->current_round_;
    this->current_round_ = 0;
    this->current_circle_ = 0;
    this->flag_ = entity::TaskCircleRoundState::INVALID;
    old += num;
    this->total_round_ = old >= 3 ? 3 : old;
    LOG_ERROR("获取环数(%d),(%d),状态(%d)", this->total_round_, this->current_round_,
        this->flag_);
  }
}

void TaskCircle::OnEvent(event::EventType::type type, 
    const void *data, size_t size) {
  if(this->task_ != NULL) {
    this->task_->OnEvent(type, data, size);
  }
}

bool TaskCircle::CheckGather(core::int32 template_id) {
  if(this->task_ == NULL) {
    return false;
  }
  return this->task_->CheckGather(template_id);
}

bool TaskCircle::CheckSummonNpc(core::int32 template_id) {
  if(this->task_ == NULL) {
    return false;
  }
  return this->task_->CheckSummonNpc(template_id);
}

} // namespace task

} // namespace server

} // namespace game

