//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-08 19:04:54.
// File name: check_birth_place_condition.cc
//
// Description:
// Define class CheckBirthPlaceCondition.
//

#include "game_server/server/extension/ai/check_birth_place_condition.h"

#include "game_server/server/game_npc.h"
#include "game_server/server/extension/ai/black_board.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

const std::string CheckBirthPlaceCondition::kTypeName_ = "check_birth_place";

CheckBirthPlaceCondition::CheckBirthPlaceCondition() {}
CheckBirthPlaceCondition::~CheckBirthPlaceCondition() {}

bool CheckBirthPlaceCondition::LoadConfigure(TiXmlElement *node) {
  return true;
}

bool CheckBirthPlaceCondition::DoCheckEvalute(BlackBoard *black_board) const {
  if(black_board == NULL) {
    return false;
  }

  GameRole *role = black_board->GetGameRole();
  if(role == NULL) {
    return false;
  }

  if(role->GetType() != entity::EntityType::TYPE_NPC) {
    return false;
  }

  core::uint32 birth_x = 0, birth_y = 0;
  ((GameNpc *)role)->GetBornPosition(birth_x, birth_y);

  if(birth_x == role->GetPosx() && birth_y == role->GetPosy()) {
    return true;
  }

  return false;
}

}  // namespace ai

}  // namespace server

}  // namespace game

