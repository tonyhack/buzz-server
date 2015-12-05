//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-11-21 16:46:00.
// File name: mp_condition.cc
//
// Description:
// Define mp condition.
//

#include "game_server/server/extension/buff/mp_condition.h"

#include "game_server/server/game_role.h"
#include "game_server/server/event/game_role_event_types.h"
#include "game_server/server/extension/buff/buff_role.h"
#include "game_server/server/extension/buff/buff_role_manager.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace buff {

/////////////////////////////////////////////////////////
// MpPercentBelowCondition
MpPercentBelowCondition::MpPercentBelowCondition() {}
MpPercentBelowCondition::~MpPercentBelowCondition() {}

bool MpPercentBelowCondition::Initialize(core::int32 id, BuffRole *role) {
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

  core::int32 max_mp = game_role->GetAttribute(entity::RoleAoiFields::MAX_MP);
  core::int32 current_mp = game_role->GetAttribute(entity::RoleAoiFields::CURRENT_MP);

	this->passed_ = current_mp < (max_mp * this->param1_ / 100);

  return true;
}

void MpPercentBelowCondition::Finalize() {
  Condition::Finalize();
}

bool MpPercentBelowCondition::OnEvent(event::EventType::type type,
    const void *data, size_t size) {
  switch(type) {
    case event::EventType::EVENT_ROLE_CHANGE_MP:
      return this->OnEventChangeMp(data, size);
      break;
    default:
      break;
  }

  return false;
}

bool MpPercentBelowCondition::OnEventChangeMp(const void *data, size_t size) {
  if(data == NULL || size != sizeof(event::EventRoleChangeMp)) {
    LOG_ERROR("参数错误");
    return false;
  }

  event::EventRoleChangeMp *event = (event::EventRoleChangeMp *)data;

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

  core::int32 max_mp = game_role->GetAttribute(entity::RoleAoiFields::MAX_MP);
  core::int32 current_mp = game_role->GetAttribute(entity::RoleAoiFields::CURRENT_MP);

	this->passed_ = current_mp < (max_mp * this->param1_ / 100);

  return true;
}


/////////////////////////////////////////////////////////
// MpPercentAboveCondition
MpPercentAboveCondition::MpPercentAboveCondition() {}
MpPercentAboveCondition::~MpPercentAboveCondition() {}

bool MpPercentAboveCondition::Initialize(core::int32 id, BuffRole *role) {
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

  core::int32 max_mp = game_role->GetAttribute(entity::RoleAoiFields::MAX_MP);
  core::int32 current_mp = game_role->GetAttribute(entity::RoleAoiFields::CURRENT_MP);

	this->passed_ = current_mp > (max_mp * this->param1_ / 100);

  return true;
}

void MpPercentAboveCondition::Finalize() {
  Condition::Finalize();
}

bool MpPercentAboveCondition::OnEvent(event::EventType::type type,
    const void *data, size_t size) {
  switch(type) {
    case event::EventType::EVENT_ROLE_CHANGE_MP:
      return this->OnEventChangeMp(data, size);
      break;
    default:
      break;
  }

  return false;
}

bool MpPercentAboveCondition::OnEventChangeMp(const void *data, size_t size) {
  if(data == NULL || size != sizeof(event::EventRoleChangeMp)) {
    LOG_ERROR("参数错误");
    return false;
  }

  event::EventRoleChangeMp *event = (event::EventRoleChangeMp *)data;

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

  core::int32 max_mp = game_role->GetAttribute(entity::RoleAoiFields::MAX_MP);
  core::int32 current_mp = game_role->GetAttribute(entity::RoleAoiFields::CURRENT_MP);

	this->passed_ = current_mp > (max_mp * this->param1_ / 100);

  return true;
}


/////////////////////////////////////////////////////////
// MpPercentEqualCondition
MpPercentEqualCondition::MpPercentEqualCondition() {}
MpPercentEqualCondition::~MpPercentEqualCondition() {}

bool MpPercentEqualCondition::Initialize(core::int32 id, BuffRole *role) {
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

  core::int32 max_mp = game_role->GetAttribute(entity::RoleAoiFields::MAX_MP);
  core::int32 current_mp = game_role->GetAttribute(entity::RoleAoiFields::CURRENT_MP);

	this->passed_ = current_mp == (max_mp * this->param1_ / 100);

  return true;
}

void MpPercentEqualCondition::Finalize() {
  Condition::Finalize();
}

bool MpPercentEqualCondition::OnEvent(event::EventType::type type,
    const void *data, size_t size) {
  switch(type) {
    case event::EventType::EVENT_ROLE_CHANGE_MP:
      return this->OnEventChangeMp(data, size);
      break;
    default:
      break;
  }

  return false;
}

bool MpPercentEqualCondition::OnEventChangeMp(const void *data, size_t size) {
  if(data == NULL || size != sizeof(event::EventRoleChangeMp)) {
    LOG_ERROR("参数错误");
    return false;
  }

  event::EventRoleChangeMp *event = (event::EventRoleChangeMp *)data;

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

  core::int32 max_mp = game_role->GetAttribute(entity::RoleAoiFields::MAX_MP);
  core::int32 current_mp = game_role->GetAttribute(entity::RoleAoiFields::CURRENT_MP);

	this->passed_ = current_mp == (max_mp * this->param1_ / 100);

  return true;
}


/////////////////////////////////////////////////////////
// MpValueBelowCondition
MpValueBelowCondition::MpValueBelowCondition() {}
MpValueBelowCondition::~MpValueBelowCondition() {}

bool MpValueBelowCondition::Initialize(core::int32 id, BuffRole *role) {
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

  core::int32 current_mp = game_role->GetAttribute(entity::RoleAoiFields::CURRENT_MP);

	this->passed_ = current_mp < this->param1_;

  return true;
}

void MpValueBelowCondition::Finalize() {
  Condition::Finalize();
}

bool MpValueBelowCondition::OnEvent(event::EventType::type type,
    const void *data, size_t size) {
  switch(type) {
    case event::EventType::EVENT_ROLE_CHANGE_MP:
      return this->OnEventChangeMp(data, size);
      break;
    default:
      break;
  }

  return false;
}

bool MpValueBelowCondition::OnEventChangeMp(const void *data, size_t size) {
  if(data == NULL || size != sizeof(event::EventRoleChangeMp)) {
    LOG_ERROR("参数错误");
    return false;
  }

  event::EventRoleChangeMp *event = (event::EventRoleChangeMp *)data;

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

  core::int32 current_mp = game_role->GetAttribute(entity::RoleAoiFields::CURRENT_MP);

	this->passed_ = current_mp < this->param1_;

  return true;
}


/////////////////////////////////////////////////////////
// MpValueAboveCondition
MpValueAboveCondition::MpValueAboveCondition() {}
MpValueAboveCondition::~MpValueAboveCondition() {}

bool MpValueAboveCondition::Initialize(core::int32 id, BuffRole *role) {
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

  core::int32 current_mp = game_role->GetAttribute(entity::RoleAoiFields::CURRENT_MP);

	this->passed_ = current_mp > this->param1_;

  return true;
}

void MpValueAboveCondition::Finalize() {
  Condition::Finalize();
}

bool MpValueAboveCondition::OnEvent(event::EventType::type type,
    const void *data, size_t size) {
  switch(type) {
    case event::EventType::EVENT_ROLE_CHANGE_MP:
      return this->OnEventChangeMp(data, size);
      break;
    default:
      break;
  }

  return false;
}

bool MpValueAboveCondition::OnEventChangeMp(const void *data, size_t size) {
  if(data == NULL || size != sizeof(event::EventRoleChangeMp)) {
    LOG_ERROR("参数错误");
    return false;
  }

  event::EventRoleChangeMp *event = (event::EventRoleChangeMp *)data;

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

  core::int32 current_mp = game_role->GetAttribute(entity::RoleAoiFields::CURRENT_MP);

	this->passed_ = current_mp > this->param1_;

  return true;
}


/////////////////////////////////////////////////////////
// MpValueEqualCondition
MpValueEqualCondition::MpValueEqualCondition() {}
MpValueEqualCondition::~MpValueEqualCondition() {}

bool MpValueEqualCondition::Initialize(core::int32 id, BuffRole *role) {
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

  core::int32 current_mp = game_role->GetAttribute(entity::RoleAoiFields::CURRENT_MP);

	this->passed_ = current_mp == this->param1_;

  return true;
}

void MpValueEqualCondition::Finalize() {
  Condition::Finalize();
}

bool MpValueEqualCondition::OnEvent(event::EventType::type type,
    const void *data, size_t size) {
  switch(type) {
    case event::EventType::EVENT_ROLE_CHANGE_MP:
      return this->OnEventChangeMp(data, size);
      break;
    default:
      break;
  }

  return false;
}

bool MpValueEqualCondition::OnEventChangeMp(const void *data, size_t size) {
  if(data == NULL || size != sizeof(event::EventRoleChangeMp)) {
    LOG_ERROR("参数错误");
    return false;
  }

  event::EventRoleChangeMp *event = (event::EventRoleChangeMp *)data;

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

  core::int32 current_mp = game_role->GetAttribute(entity::RoleAoiFields::CURRENT_MP);

	this->passed_ = current_mp == this->param1_;

  return true;
}

}  // namespace buff

}  // namespace server

}  // namespace game

