//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-11-21 16:46:00.
// File name: hp_condition.cc
//
// Description:
// Define hp condition.
//

#include "game_server/server/extension/buff/hp_condition.h"

#include "game_server/server/game_role.h"
#include "game_server/server/event/game_role_event_types.h"
#include "game_server/server/extension/buff/buff_role.h"
#include "game_server/server/extension/buff/buff_role_manager.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace buff {

/////////////////////////////////////////////////////////
// HpPercentBelowCondition
HpPercentBelowCondition::HpPercentBelowCondition() {}
HpPercentBelowCondition::~HpPercentBelowCondition() {}

bool HpPercentBelowCondition::Initialize(core::int32 id, BuffRole *role) {
  if(role == NULL) {
    LOG_ERROR("参数错误");
    return false;
  }

  if(Condition::Initialize(id, role) == false) {
    LOG_ERROR("初始化 Condition(%d) 失败", id);
    return false;
  }

  GameRole *game_role = role->GetRole();
  if(game_role == NULL) {
    LOG_ERROR("获取 GameRole 失败");
    return false;
  }

  core::int32 max_hp = game_role->GetAttribute(entity::RoleAoiFields::MAX_HP);
  core::int32 current_hp = game_role->GetAttribute(entity::RoleAoiFields::CURRENT_HP);

	this->passed_ = current_hp < (max_hp * this->param1_ / 100);

  return true;
}

void HpPercentBelowCondition::Finalize() {
  Condition::Finalize();
}

bool HpPercentBelowCondition::OnEvent(event::EventType::type type,
    const void *data, size_t size) {
  switch(type) {
    case event::EventType::EVENT_ROLE_CHANGE_HP:
      return this->OnEventChangeHp(data, size);
      break;
    default:
      break;
  }

  return false;
}

bool HpPercentBelowCondition::OnEventChangeHp(const void *data, size_t size) {
  if(data == NULL || size != sizeof(event::EventRoleChangeHp)) {
    LOG_ERROR("参数错误");
    return false;
  }

  event::EventRoleChangeHp *event = (event::EventRoleChangeHp *)data;

  BuffRole *role = BuffRoleManager::GetInstance()->GetRole(event->type_,
      event->id_);
  if(role == NULL) {
    LOG_ERROR("获取 BuffRole(%d, %lu) 失败", event->type_, event->id_);
    return false;
  }

  GameRole *game_role = role->GetRole();
  if(game_role == NULL) {
    LOG_ERROR("获取 GameRole(%d, %lu) 失败", event->type_, event->id_);
    return false;
  }

  core::int32 max_hp = game_role->GetAttribute(entity::RoleAoiFields::MAX_HP);
  core::int32 current_hp = game_role->GetAttribute(entity::RoleAoiFields::CURRENT_HP);

	this->passed_ = current_hp < (max_hp * this->param1_ / 100);

  return true;
}


/////////////////////////////////////////////////////////
// HpPercentAboveCondition
HpPercentAboveCondition::HpPercentAboveCondition() {}
HpPercentAboveCondition::~HpPercentAboveCondition() {}

bool HpPercentAboveCondition::Initialize(core::int32 id, BuffRole *role) {
  if(role == NULL) {
    LOG_ERROR("参数错误");
    return false;
  }

  if(Condition::Initialize(id, role) == false) {
    LOG_ERROR("初始化 Condition(%d) 失败", id);
    return false;
  }

  GameRole *game_role = role->GetRole();
  if(game_role == NULL) {
    LOG_ERROR("获取 GameRole 失败");
    return false;
  }

	core::int32 max_hp = game_role->GetAttribute(entity::RoleAoiFields::MAX_HP);
	core::int32 current_hp = game_role->GetAttribute(entity::RoleAoiFields::CURRENT_HP);

	this->passed_ = current_hp > (max_hp * this->param1_ / 100);

  return true;
}

void HpPercentAboveCondition::Finalize() {
  Condition::Finalize();
}

bool HpPercentAboveCondition::OnEvent(event::EventType::type type,
    const void *data, size_t size) {
  switch(type) {
    case event::EventType::EVENT_ROLE_CHANGE_HP:
      return this->OnEventChangeHp(data, size);
      break;
    default:
      break;
  }

  return false;
}

bool HpPercentAboveCondition::OnEventChangeHp(const void *data, size_t size) {
	if(data == NULL || size != sizeof(event::EventRoleChangeHp)) {
		LOG_ERROR("参数错误");
		return false;
	}

	event::EventRoleChangeHp *event = (event::EventRoleChangeHp *)data;

	BuffRole *role = BuffRoleManager::GetInstance()->GetRole(event->type_,
			event->id_);
	if(role == NULL) {
		LOG_ERROR("获取 BuffRole(%d, %lu) 失败", event->type_, event->id_);
		return false;
	}

  GameRole *game_role = role->GetRole();
  if(game_role == NULL) {
    LOG_ERROR("获取 GameRole(%d, %lu) 失败", event->type_, event->id_);
    return false;
  }

	core::int32 max_hp = game_role->GetAttribute(entity::RoleAoiFields::MAX_HP);
	core::int32 current_hp = game_role->GetAttribute(entity::RoleAoiFields::CURRENT_HP);

	this->passed_ = current_hp > (max_hp * this->param1_ / 100);

  return true;
}


/////////////////////////////////////////////////////////
// HpPercentEqualCondition
HpPercentEqualCondition::HpPercentEqualCondition() {}
HpPercentEqualCondition::~HpPercentEqualCondition() {}

bool HpPercentEqualCondition::Initialize(core::int32 id, BuffRole *role) {
  if(role == NULL) {
    LOG_ERROR("参数错误");
    return false;
  }

  if(Condition::Initialize(id, role) == false) {
    LOG_ERROR("初始化 Condition(%d) 失败", id);
    return false;
  }

  GameRole *game_role = role->GetRole();
  if(game_role == NULL) {
    LOG_ERROR("获取 GameRole 失败");
    return false;
  }

  core::int32 max_hp = game_role->GetAttribute(entity::RoleAoiFields::MAX_HP);
  core::int32 current_hp = game_role->GetAttribute(entity::RoleAoiFields::CURRENT_HP);

	this->passed_ = current_hp == (max_hp * this->param1_ / 100);

  return true;
}

void HpPercentEqualCondition::Finalize() {
  Condition::Finalize();
}

bool HpPercentEqualCondition::OnEvent(event::EventType::type type,
    const void *data, size_t size) {
  switch(type) {
    case event::EventType::EVENT_ROLE_CHANGE_HP:
      return this->OnEventChangeHp(data, size);
      break;
    default:
      break;
  }

  return false;
}

bool HpPercentEqualCondition::OnEventChangeHp(const void *data, size_t size) {
  if(data == NULL || size != sizeof(event::EventRoleChangeHp)) {
    LOG_ERROR("参数错误");
    return false;
  }

  event::EventRoleChangeHp *event = (event::EventRoleChangeHp *)data;

  BuffRole *role = BuffRoleManager::GetInstance()->GetRole(event->type_,
      event->id_);
  if(role == NULL) {
    LOG_ERROR("获取 BuffRole(%d, %lu) 失败", event->type_, event->id_);
    return false;
  }

  GameRole *game_role = role->GetRole();
  if(game_role == NULL) {
    LOG_ERROR("获取 GameRole(%d, %lu) 失败", event->type_, event->id_);
    return false;
  }

  core::int32 max_hp = game_role->GetAttribute(entity::RoleAoiFields::MAX_HP);
  core::int32 current_hp = game_role->GetAttribute(entity::RoleAoiFields::CURRENT_HP);

	this->passed_ = current_hp == (max_hp * this->param1_ / 100);

  return true;
}


/////////////////////////////////////////////////////////
// HpValueBelowCondition
HpValueBelowCondition::HpValueBelowCondition() {}
HpValueBelowCondition::~HpValueBelowCondition() {}

bool HpValueBelowCondition::Initialize(core::int32 id, BuffRole *role) {
  if(role == NULL) {
    LOG_ERROR("参数错误");
    return false;
  }

  if(Condition::Initialize(id, role) == false) {
    LOG_ERROR("初始化 Condition(%d) 失败", id);
    return false;
  }

  GameRole *game_role = role->GetRole();
  if(game_role == NULL) {
    LOG_ERROR("获取 GameRole 失败");
    return false;
  }

	core::int32 current_hp = game_role->GetAttribute(entity::RoleAoiFields::CURRENT_HP);

	this->passed_ = current_hp < this->param1_;

  return true;
}

void HpValueBelowCondition::Finalize() {
  Condition::Finalize();
}

bool HpValueBelowCondition::OnEvent(event::EventType::type type,
    const void *data, size_t size) {
  switch(type) {
    case event::EventType::EVENT_ROLE_CHANGE_HP:
      return this->OnEventChangeHp(data, size);
      break;
    default:
      break;
  }

  return false;
}

bool HpValueBelowCondition::OnEventChangeHp(const void *data, size_t size) {
  if(data == NULL || size != sizeof(event::EventRoleChangeHp)) {
    LOG_ERROR("参数错误");
    return false;
  }

  event::EventRoleChangeHp *event = (event::EventRoleChangeHp *)data;

  BuffRole *role = BuffRoleManager::GetInstance()->GetRole(event->type_,
      event->id_);
  if(role == NULL) {
    LOG_ERROR("获取 BuffRole(%d, %lu) 失败", event->type_, event->id_);
    return false;
  }

  GameRole *game_role = role->GetRole();
  if(game_role == NULL) {
    LOG_ERROR("获取 GameRole(%d, %lu) 失败", event->type_, event->id_);
    return false;
  }

	core::int32 current_hp = game_role->GetAttribute(entity::RoleAoiFields::CURRENT_HP);

	this->passed_ = current_hp < this->param1_;

  return true;
}


/////////////////////////////////////////////////////////
// HpValueAboveCondition
HpValueAboveCondition::HpValueAboveCondition() {}
HpValueAboveCondition::~HpValueAboveCondition() {}

bool HpValueAboveCondition::Initialize(core::int32 id, BuffRole *role) {
  if(role == NULL) {
    LOG_ERROR("参数错误");
    return false;
  }

  if(Condition::Initialize(id, role) == false) {
    LOG_ERROR("初始化 Condition(%d) 失败", id);
    return false;
  }

  GameRole *game_role = role->GetRole();
  if(game_role == NULL) {
    LOG_ERROR("获取 GameRole 失败");
    return false;
  }

	core::int32 current_hp = game_role->GetAttribute(entity::RoleAoiFields::CURRENT_HP);

	this->passed_ = current_hp > this->param1_;

  return true;
}

void HpValueAboveCondition::Finalize() {
  Condition::Finalize();
}

bool HpValueAboveCondition::OnEvent(event::EventType::type type,
    const void *data, size_t size) {
  switch(type) {
    case event::EventType::EVENT_ROLE_CHANGE_HP:
      return this->OnEventChangeHp(data, size);
      break;
    default:
      break;
  }

  return false;
}

bool HpValueAboveCondition::OnEventChangeHp(const void *data, size_t size) {
  if(data == NULL || size != sizeof(event::EventRoleChangeHp)) {
    LOG_ERROR("参数错误");
    return false;
  }

  event::EventRoleChangeHp *event = (event::EventRoleChangeHp *)data;

  BuffRole *role = BuffRoleManager::GetInstance()->GetRole(event->type_,
      event->id_);
  if(role == NULL) {
    LOG_ERROR("获取 BuffRole(%d, %lu) 失败", event->type_, event->id_);
    return false;
  }

  GameRole *game_role = role->GetRole();
  if(game_role == NULL) {
    LOG_ERROR("获取 GameRole(%d, %lu) 失败", event->type_, event->id_);
    return false;
  }

	core::int32 current_hp = game_role->GetAttribute(entity::RoleAoiFields::CURRENT_HP);

	this->passed_ = current_hp > this->param1_;

  return true;
}


/////////////////////////////////////////////////////////
// HpValueEqualCondition
HpValueEqualCondition::HpValueEqualCondition() {}
HpValueEqualCondition::~HpValueEqualCondition() {}

bool HpValueEqualCondition::Initialize(core::int32 id, BuffRole *role) {
  if(role == NULL) {
    LOG_ERROR("参数错误");
    return false;
  }

  if(Condition::Initialize(id, role) == false) {
    LOG_ERROR("初始化 Condition(%d) 失败", id);
    return false;
  }

  GameRole *game_role = role->GetRole();
  if(game_role == NULL) {
    LOG_ERROR("获取 GameRole 失败");
    return false;
  }

	core::int32 current_hp = game_role->GetAttribute(entity::RoleAoiFields::CURRENT_HP);

	this->passed_ = current_hp == this->param1_;

  return true;
}

void HpValueEqualCondition::Finalize() {
  Condition::Finalize();
}

bool HpValueEqualCondition::OnEvent(event::EventType::type type,
    const void *data, size_t size) {
  switch(type) {
    case event::EventType::EVENT_ROLE_CHANGE_HP:
      return this->OnEventChangeHp(data, size);
      break;
    default:
      break;
  }

  return false;
}

bool HpValueEqualCondition::OnEventChangeHp(const void *data, size_t size) {
  if(data == NULL || size != sizeof(event::EventRoleChangeHp)) {
    LOG_ERROR("参数错误");
    return false;
  }

  event::EventRoleChangeHp *event = (event::EventRoleChangeHp *)data;

  BuffRole *role = BuffRoleManager::GetInstance()->GetRole(event->type_,
      event->id_);
  if(role == NULL) {
    LOG_ERROR("获取 BuffRole(%d, %lu) 失败", event->type_, event->id_);
    return false;
  }

  GameRole *game_role = role->GetRole();
  if(game_role == NULL) {
    LOG_ERROR("获取 GameRole(%d, %lu) 失败", event->type_, event->id_);
    return false;
  }

	core::int32 current_hp = game_role->GetAttribute(entity::RoleAoiFields::CURRENT_HP);

	this->passed_ = current_hp == this->param1_;

  return true;
}

}  // namespace buff

}  // namespace server

}  // namespace game

