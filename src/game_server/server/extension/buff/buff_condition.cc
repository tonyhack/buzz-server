//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-11-26 11:09:05.
// File name: buff_condition.cc
//
// Description:
// Define buff condition.
//

#include "game_server/server/extension/buff/buff_condition.h"

#include "game_server/server/game_role.h"
#include "game_server/server/event/game_buff_event_types.h"
#include "game_server/server/extension/buff/buff_role.h"
#include "game_server/server/extension/buff/buff_role_manager.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace buff {

/////////////////////////////////////////////////////
// BuffCondition
BuffCondition::BuffCondition() {}
BuffCondition::~BuffCondition() {}

bool BuffCondition::Initialize(core::int32 id, BuffRole *role) {
  if(role == NULL) {
    LOG_ERROR("参数错误");
    return false;
  }

  if(Condition::Initialize(id, role) == false) {
    LOG_ERROR("初始化 Condition(%d) 失败", id);
    return false;
  }

  if(role->GetBuff(this->param1_) != NULL) {
    this->passed_ = true;
  }

  return true;
}

void BuffCondition::Finalize() {
  Condition::Finalize();
}

bool BuffCondition::OnEvent(event::EventType::type type,
    const void *data, size_t size) {
  switch(type) {
    case event::EventType::EVENT_BUFF_START:
      return this->OnEventBuffStart(data, size);
    case event::EventType::EVENT_BUFF_END:
      return this->OnEventBuffEnd(data, size);
    default:
      break;
  }

  return false;
}

bool BuffCondition::OnEventBuffStart(const void *data, size_t size) {
  if(data == NULL || size != sizeof(event::EventBuffStart)) {
    LOG_ERROR("参数错误");
    return false;
  }

  event::EventBuffStart *event = (event::EventBuffStart *)data;

  BuffRole *role = BuffRoleManager::GetInstance()->GetRole(event->type_,
      event->id_);
  if(role == NULL) {
    LOG_ERROR("获取 BuffRole(%d, %lu) 失败", event->type_, event->id_);
    return false;
  }

  if(event->buff_ == this->param1_ && event->stack_count_ == 1) {
    this->passed_ = true;
    return true;
  }

  return false;
}

bool BuffCondition::OnEventBuffEnd(const void *data, size_t size) {
  if(data == NULL || size != sizeof(event::EventBuffEnd)) {
    LOG_ERROR("参数错误");
    return false;
  }

  event::EventBuffEnd *event = (event::EventBuffEnd *)data;

  BuffRole *role = BuffRoleManager::GetInstance()->GetRole(event->type_,
      event->id_);
  if(role == NULL) {
    LOG_ERROR("获取 BuffRole(%d, %lu) 失败", event->type_, event->id_);
    return false;
  }

  if(event->buff_ == this->param1_) {
    this->passed_ = false;
    return true;
  }

  return false;
}


/////////////////////////////////////////////////////
// BuffGroupCondition
BuffGroupCondition::BuffGroupCondition() {}
BuffGroupCondition::~BuffGroupCondition() {}

bool BuffGroupCondition::Initialize(core::int32 id, BuffRole *role) {
  if(role == NULL) {
    LOG_ERROR("参数错误");
    return false;
  }

  if(Condition::Initialize(id, role) == false) {
    LOG_ERROR("初始化 Condition(%d) 失败", id);
    return false;
  }

  if(role->CheckBuffConditionGroupExist(this->param1_) == true) {
    this->passed_ = true;
  }

  return true;
}

void BuffGroupCondition::Finalize() {
  Condition::Finalize();
}

bool BuffGroupCondition::OnEvent(event::EventType::type type,
    const void *data, size_t size) {
  switch(type) {
    case event::EventType::EVENT_BUFF_START:
      return this->OnEventBuffStart(data, size);
    case event::EventType::EVENT_BUFF_END:
      return this->OnEventBuffEnd(data, size);
    default:
      break;
  }

  return false;
}

bool BuffGroupCondition::OnEventBuffStart(const void *data, size_t size) {
  if(data == NULL || size != sizeof(event::EventBuffStart)) {
    LOG_ERROR("参数错误");
    return false;
  }

  event::EventBuffStart *event = (event::EventBuffStart *)data;

  BuffRole *role = BuffRoleManager::GetInstance()->GetRole(event->type_,
      event->id_);
  if(role == NULL) {
    LOG_ERROR("获取 BuffRole(%d, %lu) 失败", event->type_, event->id_);
    return false;
  }

  if(this->passed_ == false && role->CheckBuffConditionGroupExist(this->param1_) == true) {
    this->passed_ = true;
    return true;
  }

  return false;
}

bool BuffGroupCondition::OnEventBuffEnd(const void *data, size_t size) {
  if(data == NULL || size != sizeof(event::EventBuffEnd)) {
    LOG_ERROR("参数错误");
    return false;
  }

  event::EventBuffEnd *event = (event::EventBuffEnd *)data;

  BuffRole *role = BuffRoleManager::GetInstance()->GetRole(event->type_,
      event->id_);
  if(role == NULL) {
    LOG_ERROR("获取 BuffRole(%d, %lu) 失败", event->type_, event->id_);
    return false;
  }

  if(this->passed_ == true && role->CheckBuffConditionGroupExist(this->param1_) == false) {
    this->passed_ = false;
    return true;
  }

  return false;
}

}  // namespace buff

}  // namespace server

}  // namespace game

