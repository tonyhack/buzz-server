//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-08 18:14:35.
// File name: set_back_state_action.cc
//
// Description:
// Define class SetBackStateAction.
//

#include "game_server/server/extension/ai/set_back_state_action.h"

#include "game_server/server/extension/ai/black_board.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

const std::string SetBackStateAction::kTypeName_ = "set_back_state";

SetBackStateAction::SetBackStateAction() {}
SetBackStateAction::~SetBackStateAction() {}

bool SetBackStateAction::LoadConfigure(TiXmlElement *node) {
  if(node == NULL) {
    return false;
  }

  const char *param1 = node->Attribute("param1");
  if(param1 == NULL) {
    global::LogError("%s:%d (%s) 找不到结点[node-param1]",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(strncmp(param1, "true", sizeof("true")) == 0) {
    this->status_ = true;
  } else {
    this->status_ = false;
  }

  return true;
}

void SetBackStateAction::OnEnter(BlackBoard *black_board) {}

RunningStatus::type SetBackStateAction::OnExecute(BlackBoard *black_board) {
  if(black_board == NULL) {
    return RunningStatus::ERROR;
  }

  black_board->SetBackStatus(this->status_);

  return RunningStatus::FINISH;
}

void SetBackStateAction::OnQuit(BlackBoard *black_board) {}

}  // namespace ai

}  // namespace server

}  // namespace game

