//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-08 17:53:33.
// File name: check_back_state_condition.cc
//
// Description:
// Define class CheckBackStateCondition.
//

#include "game_server/server/extension/ai/check_back_state_condition.h"

#include "game_server/server/extension/ai/black_board.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

const std::string CheckBackStateCondition::kTypeName_ = "check_back_state";

CheckBackStateCondition::CheckBackStateCondition() {}
CheckBackStateCondition::~CheckBackStateCondition() {}

bool CheckBackStateCondition::LoadConfigure(TiXmlElement *node) {
  return true;
}

bool CheckBackStateCondition::DoCheckEvalute(BlackBoard *black_board) const {
  if(black_board == NULL) {
    return false;
  }

  return black_board->CheckBackStatus();
}

}  // namespace ai

}  // namespace server

}  // namespace game

