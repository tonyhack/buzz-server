
#include "game_server/server/extension/welfare/condition.h"

#include "global/logging.h"

namespace game {

namespace server {

namespace welfare {

Condition::Condition() {}
Condition::~Condition() {}

bool Condition::Initialize(Task *task, entity::AchieveConditionType::type type, 
      core::uint32 param1, core::uint32 param2, core::uint32 param3, core::uint32 value) {
  if(task == NULL) {
    LOG_ERROR("创建成就任务失败");
    return false;
  }
  this->task_ = task;
  this->type_ = type;
  this->param1_ = param1;
  this->param2_ = param2;
  this->param3_ = param3;
  this->value_ = value;

  this->imp_.Initialize(this);

  return true;
}

void Condition::Finalize() {
  this->task_ = NULL;
}

bool Condition::ExportConditionVar(core::int32 param1, core::int32 &param2, core::int32 &value) {
  param1 = this->param1_;
  param2 = this->param2_;
  value = this->value_;

  return true;
}

bool Condition::CheckCondition() {
  if(this->param3_ <= this->value_) {
    return true;
  }
  return false;
}

void Condition::OnEvent(event::EventType::type type, const void *data, size_t size) {
  this->imp_.OnEvent(type, data, size);
}

Task *Condition::GetTask() {
  return  this->task_;
}

void Condition::Clear() {

}

}  // namespace welfare

}  // namespace server

}  // namespace game
