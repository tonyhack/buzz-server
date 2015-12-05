//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-11-21 20:00:24.
// File name: status_condition.cc
//
// Description:
// Define status condition.
//

#include "game_server/server/extension/buff/status_condition.h"

#include "game_server/server/game_role.h"
#include "game_server/server/event/game_role_event_types.h"
#include "game_server/server/extension/buff/buff_role.h"
#include "game_server/server/extension/buff/buff_role_manager.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace buff {

//////////////////////////////////////////////////////////////////
// SoulStatusCondition

SoulStatusCondition::SoulStatusCondition() {}
SoulStatusCondition::~SoulStatusCondition() {}

bool SoulStatusCondition::Initialize(core::int32 id, BuffRole *role) {
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

  core::uint32 status = (core::uint32)game_role->GetAttribute(entity::RoleAoiFields::COMMON_STATUS);
  if(entity::RoleCommonStatus::SOUL & status) {
    this->passed_ = true;
  } else {
    this->passed_ = false;
  }

  return true;
}

void SoulStatusCondition::Finalize() {
  Condition::Finalize();
}

bool SoulStatusCondition::OnEvent(event::EventType::type type,
    const void *data, size_t size) {
  switch(type) {
    case event::EventType::EVENT_ROLE_COMMON_STATUS_CHANGE:
      return this->OnEventCommonStatusChange(data, size);
      break;
    default:
      break;
  }

  return false;
}

bool SoulStatusCondition::OnEventCommonStatusChange(const void *data, size_t size) {
  if(data == NULL || size != sizeof(event::EventRoleCommonStatusChange)) {
    LOG_ERROR("参数错误");
    return false;
  }

  event::EventRoleCommonStatusChange *event = (event::EventRoleCommonStatusChange *)data;

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

  core::uint32 status = game_role->GetAttribute(entity::RoleAoiFields::COMMON_STATUS);
  if(this->passed_ == false && (entity::RoleCommonStatus::SOUL & status) != 0) {
    this->passed_ = true;
    return true;
  } else if(this->passed_ == true && (entity::RoleCommonStatus::SOUL & status) == 0) {
    this->passed_ = false;
    return true;
  }

  return false;
}


//////////////////////////////////////////////////////////////////
// NotSoulStatusCondition

NotSoulStatusCondition::NotSoulStatusCondition() {}
NotSoulStatusCondition::~NotSoulStatusCondition() {}

bool NotSoulStatusCondition::Initialize(core::int32 id, BuffRole *role) {
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

  core::uint32 status = (core::uint32)game_role->GetAttribute(entity::RoleAoiFields::COMMON_STATUS);
  if(entity::RoleCommonStatus::SOUL & status) {
    this->passed_ = false;
  } else {
    this->passed_ = true;
  }

  return true;
}

void NotSoulStatusCondition::Finalize() {
  Condition::Finalize();
}

bool NotSoulStatusCondition::OnEvent(event::EventType::type type,
    const void *data, size_t size) {
  switch(type) {
    case event::EventType::EVENT_ROLE_COMMON_STATUS_CHANGE:
      return this->OnEventCommonStatusChange(data, size);
      break;
    default:
      break;
  }

  return false;
}

bool NotSoulStatusCondition::OnEventCommonStatusChange(const void *data, size_t size) {
  if(data == NULL || size != sizeof(event::EventRoleCommonStatusChange)) {
    LOG_ERROR("参数错误");
    return false;
  }

  event::EventRoleCommonStatusChange *event = (event::EventRoleCommonStatusChange *)data;

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

  core::uint32 status = game_role->GetAttribute(entity::RoleAoiFields::COMMON_STATUS);
  if(this->passed_ == false && (entity::RoleCommonStatus::SOUL & status) == 0) {
    this->passed_ = true;
    return true;
  } else if(this->passed_ == true && (entity::RoleCommonStatus::SOUL & status) != 0) {
    this->passed_ = false;
    return true;
  }

  return false;
}


//////////////////////////////////////////////////////////////////
// ForbidSkillStatusCondition
ForbidSkillStatusCondition::ForbidSkillStatusCondition() {}
ForbidSkillStatusCondition::~ForbidSkillStatusCondition() {}

bool ForbidSkillStatusCondition::Initialize(core::int32 id, BuffRole *role) {
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

  core::uint32 status = (core::uint32)game_role->GetAttribute(entity::RoleAoiFields::COMMON_STATUS);
  if(entity::RoleCommonStatus::FORBID_USE_SKILL & status) {
    this->passed_ = true;
  } else {
    this->passed_ = false;
  }

  return true;
}

void ForbidSkillStatusCondition::Finalize() {
  Condition::Finalize();
}

bool ForbidSkillStatusCondition::OnEvent(event::EventType::type type,
    const void *data, size_t size) {
  switch(type) {
    case event::EventType::EVENT_ROLE_COMMON_STATUS_CHANGE:
      return this->OnEventCommonStatusChange(data, size);
      break;
    default:
      break;
  }

  return false;
}

bool ForbidSkillStatusCondition::OnEventCommonStatusChange(const void *data,
    size_t size) {
  if(data == NULL || size != sizeof(event::EventRoleCommonStatusChange)) {
    LOG_ERROR("参数错误");
    return false;
  }

  event::EventRoleCommonStatusChange *event = (event::EventRoleCommonStatusChange *)data;

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

  core::uint32 status = game_role->GetAttribute(entity::RoleAoiFields::COMMON_STATUS);
  if(this->passed_ == false && (entity::RoleCommonStatus::FORBID_USE_SKILL & status) != 0) {
    this->passed_ = true;
    return true;
  } else if(this->passed_ == true && (entity::RoleCommonStatus::FORBID_USE_SKILL & status) == 0) {
    this->passed_ = false;
    return true;
  }

  return false;
}



//////////////////////////////////////////////////////////////////
// ForbidMoveStatusCondition
ForbidMoveStatusCondition::ForbidMoveStatusCondition() {}
ForbidMoveStatusCondition::~ForbidMoveStatusCondition() {}

bool ForbidMoveStatusCondition::Initialize(core::int32 id, BuffRole *role) {
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

  core::uint32 status = (core::uint32)game_role->GetAttribute(entity::RoleAoiFields::COMMON_STATUS);
  if(entity::RoleCommonStatus::FORBID_MOVE & status) {
    this->passed_ = true;
  } else {
    this->passed_ = false;
  }

  return true;
}

void ForbidMoveStatusCondition::Finalize() {
  Condition::Finalize();
}

bool ForbidMoveStatusCondition::OnEvent(event::EventType::type type,
    const void *data, size_t size) {
  switch(type) {
    case event::EventType::EVENT_ROLE_COMMON_STATUS_CHANGE:
      return this->OnEventCommonStatusChange(data, size);
      break;
    default:
      break;
  }

  return false;
}

bool ForbidMoveStatusCondition::OnEventCommonStatusChange(const void *data,
    size_t size) {
  if(data == NULL || size != sizeof(event::EventRoleCommonStatusChange)) {
    LOG_ERROR("参数错误");
    return false;
  }

  event::EventRoleCommonStatusChange *event = (event::EventRoleCommonStatusChange *)data;

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

  core::uint32 status = game_role->GetAttribute(entity::RoleAoiFields::COMMON_STATUS);
  if(this->passed_ == false && (entity::RoleCommonStatus::FORBID_MOVE & status) != 0) {
    this->passed_ = true;
    return true;
  } else if(this->passed_ == true && (entity::RoleCommonStatus::FORBID_MOVE & status) == 0) {
    this->passed_ = false;
    return true;
  }

  return false;
}


//////////////////////////////////////////////////////////////////
// ForbidItemStatusCondition
ForbidItemStatusCondition::ForbidItemStatusCondition() {}
ForbidItemStatusCondition::~ForbidItemStatusCondition() {}

bool ForbidItemStatusCondition::Initialize(core::int32 id, BuffRole *role) {
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

  core::uint32 status = (core::uint32)game_role->GetAttribute(entity::RoleAoiFields::COMMON_STATUS);
  if(entity::RoleCommonStatus::FORBID_USE_ITEM & status) {
    this->passed_ = true;
  } else {
    this->passed_ = false;
  }

  return true;
}

void ForbidItemStatusCondition::Finalize() {
  Condition::Finalize();
}

bool ForbidItemStatusCondition::OnEvent(event::EventType::type type,
    const void *data, size_t size) {
  switch(type) {
    case event::EventType::EVENT_ROLE_COMMON_STATUS_CHANGE:
      return this->OnEventCommonStatusChange(data, size);
      break;
    default:
      break;
  }

  return false;
}

bool ForbidItemStatusCondition::OnEventCommonStatusChange(const void *data,
    size_t size) {
  if(data == NULL || size != sizeof(event::EventRoleCommonStatusChange)) {
    LOG_ERROR("参数错误");
    return false;
  }

  event::EventRoleCommonStatusChange *event = (event::EventRoleCommonStatusChange *)data;

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

  core::uint32 status = game_role->GetAttribute(entity::RoleAoiFields::COMMON_STATUS);
  if(this->passed_ == false && (entity::RoleCommonStatus::FORBID_USE_ITEM & status) != 0) {
    this->passed_ = true;
    return true;
  } else if(this->passed_ == true && (entity::RoleCommonStatus::FORBID_USE_ITEM & status) == 0) {
    this->passed_ = false;
    return true;
  }

  return false;
}

}  // namespace buff

}  // namespace server

}  // namespace game

