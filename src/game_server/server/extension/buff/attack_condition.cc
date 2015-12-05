//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-11-21 17:57:43.
// File name: attack_condition.cc
//
// Description:
// Define attack condition.
//

#include "game_server/server/extension/buff/attack_condition.h"

#include "game_server/server/event/game_skill_event_types.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace buff {

/////////////////////////////////////////////////////////
// AttackHitCondition
AttackHitCondition::AttackHitCondition() {}
AttackHitCondition::~AttackHitCondition() {}

bool AttackHitCondition::Initialize(core::int32 id, BuffRole *role) {
  if(Condition::Initialize(id, role) == false) {
    LOG_ERROR("初始化 Condition(%d) 失败", id);
    return false;
  }

  return true;
}

void AttackHitCondition::Finalize() {
  Condition::Finalize();
}

bool AttackHitCondition::OnEvent(event::EventType::type type,
    const void *data, size_t size) {
  switch(type) {
    case event::EventType::EVENT_SKILL_ATTACK_HIT:
      return this->OnEventAttackHit(data, size);
      break;
    default:
      break;
  }

  return false;
}

bool AttackHitCondition::OnEventAttackHit(const void *data, size_t size) {
  if(data == NULL || size != sizeof(event::EventSkillAttackHit)) {
    LOG_ERROR("参数错误");
    return false;
  }

  this->passed_ = true;

  return true;
}


/////////////////////////////////////////////////////////
// AttackCritCondition
AttackCritCondition::AttackCritCondition() {}
AttackCritCondition::~AttackCritCondition() {}

bool AttackCritCondition::Initialize(core::int32 id, BuffRole *role) {
  if(Condition::Initialize(id, role) == false) {
    LOG_ERROR("初始化 Condition(%d) 失败", id);
    return false;
  }

  return true;
}

void AttackCritCondition::Finalize() {
  Condition::Finalize();
}

bool AttackCritCondition::OnEvent(event::EventType::type type,
    const void *data, size_t size) {
  switch(type) {
    case event::EventType::EVENT_SKILL_ATTACK_HIT:
      return this->OnEventAttackHit(data, size);
      break;
    default:
      break;
  }

  return false;
}

bool AttackCritCondition::OnEventAttackHit(const void *data, size_t size) {
  if(data == NULL || size != sizeof(event::EventSkillAttackHit)) {
    LOG_ERROR("参数错误");
    return false;
  }

	event::EventSkillAttackHit *event = (event::EventSkillAttackHit *)data;

	if(event->hit_type_ == entity::SkillHitType::CRIT) {
		this->passed_ = true;
    return true;
	}

  return false;
}


/////////////////////////////////////////////////////////
// AttackDodgeCondition
AttackDodgeCondition::AttackDodgeCondition() {}
AttackDodgeCondition::~AttackDodgeCondition() {}

bool AttackDodgeCondition::Initialize(core::int32 id, BuffRole *role) {
  if(Condition::Initialize(id, role) == false) {
    LOG_ERROR("初始化 Condition(%d) 失败", id);
    return false;
  }

  return true;
}

void AttackDodgeCondition::Finalize() {
  Condition::Finalize();
}

bool AttackDodgeCondition::OnEvent(event::EventType::type type,
   const void *data, size_t size) {
  switch(type) {
    case event::EventType::EVENT_SKILL_ATTACK_HIT:
      return this->OnEventAttackHit(data, size);
      break;
    default:
      break;
  }

  return false;
}

bool AttackDodgeCondition::OnEventAttackHit(const void *data, size_t size) {
  if(data == NULL || size != sizeof(event::EventSkillAttackHit)) {
    LOG_ERROR("参数错误");
    return false;
  }

	event::EventSkillAttackHit *event = (event::EventSkillAttackHit *)data;

	if(event->hit_type_ == entity::SkillHitType::DODGE) {
		this->passed_ = true;
    return true;
	}

  return false;
}


/////////////////////////////////////////////////////////
// AttackIgnoreDefenceCondition
AttackIgnoreDefenceCondition::AttackIgnoreDefenceCondition() {}
AttackIgnoreDefenceCondition::~AttackIgnoreDefenceCondition() {}

bool AttackIgnoreDefenceCondition::Initialize(core::int32 id, BuffRole *role) {
  if(Condition::Initialize(id, role) == false) {
    LOG_ERROR("初始化 Condition(%d) 失败", id);
    return false;
  }

  return true;
}

void AttackIgnoreDefenceCondition::Finalize() {
  Condition::Finalize();
}

bool AttackIgnoreDefenceCondition::OnEvent(event::EventType::type type,
    const void *data, size_t size) {
  switch(type) {
    case event::EventType::EVENT_SKILL_ATTACK_HIT:
      return this->OnEventAttackHit(data, size);
      break;
    default:
      break;
  }

  return false;
}

bool AttackIgnoreDefenceCondition::OnEventAttackHit(const void *data, size_t size) {
  if(data == NULL || size != sizeof(event::EventSkillAttackHit)) {
    LOG_ERROR("参数错误");
    return false;
  }

	event::EventSkillAttackHit *event = (event::EventSkillAttackHit *)data;

	if(event->hit_type_ == entity::SkillHitType::IGNORE_DEF) {
		this->passed_ = true;
    return true;
	}

  return false;
}


/////////////////////////////////////////////////////////
// AttackedHitCondition
AttackedHitCondition::AttackedHitCondition() {}
AttackedHitCondition::~AttackedHitCondition() {}

bool AttackedHitCondition::Initialize(core::int32 id, BuffRole *role) {
  if(Condition::Initialize(id, role) == false) {
    LOG_ERROR("初始化 Condition(%d) 失败", id);
    return false;
  }

  return true;
}

void AttackedHitCondition::Finalize() {
  Condition::Finalize();
}

bool AttackedHitCondition::OnEvent(event::EventType::type type,
    const void *data, size_t size) {
  switch(type) {
    case event::EventType::EVENT_SKILL_ATTACKED_HIT:
      return this->OnEventAttackedHit(data, size);
      break;
    default:
      break;
  }

  return false;
}

bool AttackedHitCondition::OnEventAttackedHit(const void *data, size_t size) {
  if(data == NULL || size != sizeof(event::EventSkillAttackedHit)) {
    LOG_ERROR("参数错误");
    return false;
  }

  this->passed_ = true;

  return true;
}


/////////////////////////////////////////////////////////
// AttackedCritCondition
AttackedCritCondition::AttackedCritCondition() {}
AttackedCritCondition::~AttackedCritCondition() {}

bool AttackedCritCondition::Initialize(core::int32 id, BuffRole *role) {
  if(Condition::Initialize(id, role) == false) {
    LOG_ERROR("初始化 Condition(%d) 失败", id);
    return false;
  }

  return true;
}

void AttackedCritCondition::Finalize() {
  Condition::Finalize();
}

bool AttackedCritCondition::OnEvent(event::EventType::type type,
    const void *data, size_t size) {
  switch(type) {
    case event::EventType::EVENT_SKILL_ATTACKED_HIT:
      return this->OnEventAttackedHit(data, size);
      break;
    default:
      break;
  }

  return false;
}

bool AttackedCritCondition::OnEventAttackedHit(const void *data, size_t size) {
  if(data == NULL || size != sizeof(event::EventSkillAttackedHit)) {
    LOG_ERROR("参数错误");
    return false;
  }

	event::EventSkillAttackedHit *event = (event::EventSkillAttackedHit *)data;

	if(event->hit_type_ == entity::SkillHitType::CRIT) {
		this->passed_ = true;
    return true;
	}

  return false;
}


/////////////////////////////////////////////////////////
// AttackedDodgeCondition
AttackedDodgeCondition::AttackedDodgeCondition() {}
AttackedDodgeCondition::~AttackedDodgeCondition() {}

bool AttackedDodgeCondition::Initialize(core::int32 id, BuffRole *role) {
  if(Condition::Initialize(id, role) == false) {
    LOG_ERROR("初始化 Condition(%d) 失败", id);
    return false;
  }

  return true;
}

void AttackedDodgeCondition::Finalize() {
  Condition::Finalize();
}

bool AttackedDodgeCondition::OnEvent(event::EventType::type type,
    const void *data, size_t size) {
  switch(type) {
    case event::EventType::EVENT_SKILL_ATTACKED_HIT:
      return this->OnEventAttackedHit(data, size);
      break;
    default:
      break;
  }

  return false;
}

bool AttackedDodgeCondition::OnEventAttackedHit(const void *data, size_t size) {
  if(data == NULL || size != sizeof(event::EventSkillAttackedHit)) {
    LOG_ERROR("参数错误");
    return false;
  }

	event::EventSkillAttackedHit *event = (event::EventSkillAttackedHit *)data;

	if(event->hit_type_ == entity::SkillHitType::DODGE) {
		this->passed_ = true;
    return true;
	}

  return false;
}


/////////////////////////////////////////////////////////
// AttackedIgnoreDefenceCondition
AttackedIgnoreDefenceCondition::AttackedIgnoreDefenceCondition() {}
AttackedIgnoreDefenceCondition::~AttackedIgnoreDefenceCondition() {}

bool AttackedIgnoreDefenceCondition::Initialize(core::int32 id, BuffRole *role) {
  if(Condition::Initialize(id, role) == false) {
    LOG_ERROR("初始化 Condition(%d) 失败", id);
    return false;
  }

  return true;
}

void AttackedIgnoreDefenceCondition::Finalize() {
  Condition::Finalize();
}

bool AttackedIgnoreDefenceCondition::OnEvent(event::EventType::type type,
    const void *data, size_t size) {
  switch(type) {
    case event::EventType::EVENT_SKILL_ATTACKED_HIT:
      return this->OnEventAttackedHit(data, size);
      break;
    default:
      break;
  }

  return false;
}

bool AttackedIgnoreDefenceCondition::OnEventAttackedHit(const void *data, size_t size) {
  if(data == NULL || size != sizeof(event::EventSkillAttackedHit)) {
    LOG_ERROR("参数错误");
    return false;
  }

	event::EventSkillAttackedHit *event = (event::EventSkillAttackedHit *)data;

	if(event->hit_type_ == entity::SkillHitType::IGNORE_DEF) {
		this->passed_ = true;
    return true;
	}

  return false;
}

}  // namespace buff

}  // namespace server

}  // namespace game

