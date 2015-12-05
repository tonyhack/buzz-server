//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-06 15:34:16.
// File name: set_var_action.cc
//
// Description:
// Define class SetVarAction.
//

#include "game_server/server/extension/ai/set_var_action.h"

#include "game_server/server/extension/ai/black_board.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

const std::string SetVarAction::kTypeName_ = "set_var";

SetVarAction::SetVarAction() {}
SetVarAction::~SetVarAction() {}

bool SetVarAction::LoadConfigure(TiXmlElement *node) {
  if(node == NULL) {
    LOG_ERROR("参数错误");
    return false;
  }

  const char *var_str = node->Attribute("param1");
  if(var_str == NULL) {
    LOG_ERROR("找不到属性 [node-param1]");
    return false;
  }
  this->var_ = var_str;

  int value = 0;
  if(node->Attribute("param2", &value) == NULL) {
    LOG_ERROR("找不到属性 [node-param2]");
    return false;
  }

  this->value_ = value;

  return true;
}

void SetVarAction::OnEnter(BlackBoard *black_board) {}

RunningStatus::type SetVarAction::OnExecute(BlackBoard *black_board) {
  if(black_board == NULL) {
    return RunningStatus::ERROR;
  }

  black_board->GetVariables().Set(this->var_, this->value_);

  return RunningStatus::FINISH;
}

void SetVarAction::OnQuit(BlackBoard *black_board) {}


}  // namespace ai

}  // namespace server

}  // namespace game

