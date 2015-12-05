//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-17 17:36:56.
// File name: check_threat_condition.cc
//
// Description:
// Define class CheckThreatCondition.
//

#include "game_server/server/extension/ai/check_threat_condition.h"

#include "game_server/server/extension/ai/black_board.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

const std::string CheckThreatCondition::kTypeName_ = "check_threat";

CheckThreatCondition::CheckThreatCondition() {}
CheckThreatCondition::~CheckThreatCondition() {}

bool CheckThreatCondition::LoadConfigure(TiXmlElement *node) {
  return true;
}

bool CheckThreatCondition::DoCheckEvalute(BlackBoard *black_board) const {
  if(black_board == NULL) {
    return false;
  }

  return black_board->GetThreat().GetSize() > 0;
}

}  // namespace ai

}  // namespace server

}  // namespace game

