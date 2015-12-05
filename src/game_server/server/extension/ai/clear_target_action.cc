//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-09 13:51:21.
// File name: clear_target_action.cc
//
// Description:
// Define class ClearTargetAction.
//

#include "game_server/server/extension/ai/clear_target_action.h"

#include "game_server/server/extension/ai/black_board.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

const std::string ClearTargetAction::kTypeName_ = "clear_target";

ClearTargetAction::ClearTargetAction() {}
ClearTargetAction::~ClearTargetAction() {}

bool ClearTargetAction::LoadConfigure(TiXmlElement *node) {
  return true;
}

void ClearTargetAction::OnEnter(BlackBoard *black_board) {}

RunningStatus::type ClearTargetAction::OnExecute(BlackBoard *black_board) {
  if(black_board == NULL) {
    return RunningStatus::ERROR;
  }

  entity::EntityType::type target_type = entity::EntityType::TYPE_MAX;
  core::uint64 target_id = 0;
  black_board->GetTarget(target_type, target_id);

  if(target_type == entity::EntityType::TYPE_ACTOR) {
    black_board->GetThreat().Remove(target_id);
  }

  black_board->SetTarget(entity::EntityType::TYPE_MAX, 0);

  return RunningStatus::FINISH;
}

void ClearTargetAction::OnQuit(BlackBoard *black_board) {}

}  // namespace ai

}  // namespace server

}  // namespace game

