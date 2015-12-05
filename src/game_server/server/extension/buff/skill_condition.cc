//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-11-26 14:22:27.
// File name: skill_condition.cc
//
// Description:
// Define skill condition.
//

#include "game_server/server/extension/buff/skill_condition.h"

#include "game_server/server/game_role.h"
#include "game_server/server/event/game_skill_event_types.h"
#include "game_server/server/extension/buff/buff_role.h"
#include "game_server/server/extension/buff/buff_role_manager.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace buff {

///////////////////////////////////////
// SkillUseCondition

SkillUseCondition::SkillUseCondition() {}
SkillUseCondition::~SkillUseCondition() {}

bool SkillUseCondition::Initialize(core::int32 id, BuffRole *role) {
  if(role == NULL) {
    LOG_ERROR("参数错误");
    return false;
  }

  if(Condition::Initialize(id, role) == false) {
    LOG_ERROR("初始化 Condition(%d) 失败", id);
    return false;
  }

  this->passed_ = false;

  return true;
}

void SkillUseCondition::Finalize() {
  Condition::Finalize();
}


bool SkillUseCondition::OnEvent(event::EventType::type type,
    const void *data, size_t size) {
  switch(type) {
    case event::EventType::EVENT_SKILL_USE_SKILL:
      return this->OnEventUseSkill(data, size);
    default:
      break;
  }

  return false;
}


void SkillUseCondition::OnExecute() {
  this->passed_ = false;
}


bool SkillUseCondition::OnEventUseSkill(const void *data, size_t size) {
  if(data == NULL || size != sizeof(event::EventUseSkill)) {
    LOG_ERROR("参数错误");
    return false;
  }

  event::EventUseSkill *event = (event::EventUseSkill *)data;

  if(event->skill_id_ == this->param1_) {
    this->passed_ = true;
    return true;
  }

  return false;
}


///////////////////////////////////////
// SkillGroupUseCondition
SkillGroupUseCondition::SkillGroupUseCondition() {}
SkillGroupUseCondition::~SkillGroupUseCondition() {}

bool SkillGroupUseCondition::Initialize(core::int32 id, BuffRole *role) {
  if(role == NULL) {
    LOG_ERROR("参数错误");
    return false;
  }

  if(Condition::Initialize(id, role) == false) {
    LOG_ERROR("初始化 Condition(%d) 失败", id);
    return false;
  }

  this->passed_ = false;

  return true;
}

void SkillGroupUseCondition::Finalize() {
  Condition::Finalize();
}


bool SkillGroupUseCondition::OnEvent(event::EventType::type type,
    const void *data, size_t size) {
  switch(type) {
    case event::EventType::EVENT_SKILL_USE_SKILL:
      return this->OnEventUseSkill(data, size);
    default:
      break;
  }

  return false;
}


void SkillGroupUseCondition::OnExecute() {
  this->passed_ = false;
}


bool SkillGroupUseCondition::OnEventUseSkill(const void *data, size_t size) {
  if(data == NULL || size != sizeof(event::EventUseSkill)) {
    LOG_ERROR("参数错误");
    return false;
  }

  event::EventUseSkill *event = (event::EventUseSkill *)data;

  if(event->skill_group_ == this->param1_) {
    this->passed_ = true;
    return true;
  }

  return false;
}


}  // namespace buff

}  // namespace server

}  // namespace game

