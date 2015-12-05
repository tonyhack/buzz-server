//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-17 13:54:42.
// File name: change_attr_action.cc
//
// Description:
// Define class ChangeAttrAction.
//

#include "game_server/server/extension/ai/change_attr_action.h"

#include "game_server/server/extension_manager.h"
#include "game_server/server/game_role.h"
#include "game_server/server/extension/ai/black_board.h"
#include "global/logging.h" 

namespace game {

namespace server {

namespace ai {

const std::string ChangeAttrAction::kTypeName_ = "change_attr";

ChangeAttrAction::ChangeAttrAction() {}
ChangeAttrAction::~ChangeAttrAction() {}

bool ChangeAttrAction::LoadConfigure(TiXmlElement *node) {
  if(node == NULL) {
    LOG_ERROR("参数错误");
    return false;
  }

  const char *str = node->Attribute("param1");
  if(str == NULL) {
    LOG_ERROR("找不到属性[node->param1]");
    return false;
  }
  if(strncmp(str, "HP", sizeof("HP")) == 0) {
    this->type_ = AttrType::HP;
  } else if(strncmp(str, "MP", sizeof("MP")) == 0) {
    this->type_ = AttrType::MP;
  } else {
    LOG_ERROR("参数 param1(%s) 不合法", str);
    return false;
  }

  if(node->Attribute("param2", (int *)&this->increment_) == NULL) {
    LOG_ERROR("找不到属性[node-param2]");
    return false;
  }

  return true;
}

void ChangeAttrAction::OnEnter(BlackBoard *black_board) {}

RunningStatus::type ChangeAttrAction::OnExecute(BlackBoard *black_board) {
  if(black_board == NULL) {
    return RunningStatus::ERROR;
  }

  GameRole *role = black_board->GetGameRole();
  if(role == NULL) {
    return RunningStatus::ERROR;
  }

  core::int32 current_value = 0;
  core::int32 max_value = 0;

  if(this->type_ == AttrType::HP) {
    current_value = role->GetAttribute(entity::RoleAoiFields::CURRENT_HP);
    max_value = role->GetAttribute(entity::RoleAoiFields::MAX_HP);
    if(current_value == max_value) {
      return RunningStatus::FINISH;
    }
    if(this->increment_ == 0) {
      role->SetAttribute(entity::RoleAoiFields::CURRENT_HP, max_value);
    } else {
      role->SetAttribute(entity::RoleAoiFields::CURRENT_HP, current_value + this->increment_);
    }
  } else if(this->type_ == AttrType::MP) {
    current_value = role->GetAttribute(entity::RoleAoiFields::CURRENT_MP);
    max_value = role->GetAttribute(entity::RoleAoiFields::MAX_MP);
    if(current_value == max_value) {
      return RunningStatus::FINISH;
    }
    if(this->increment_ == 0) {
      role->SetAttribute(entity::RoleAoiFields::CURRENT_MP, max_value);
    } else {
      role->SetAttribute(entity::RoleAoiFields::CURRENT_MP, current_value + this->increment_);
    }
  } else {
    return RunningStatus::ERROR;
  }

  return RunningStatus::FINISH;
}

void ChangeAttrAction::OnQuit(BlackBoard *black_board) {}

}  // namespace ai

}  // namespace server

}  // namespace game

