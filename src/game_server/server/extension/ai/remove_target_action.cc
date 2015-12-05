//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-09 17:36:04.
// File name: remove_target_action.cc
//
// Description:
// Define class RemoveTargetAction.
//

#include "game_server/server/extension/ai/remove_target_action.h"

#include "game_server/server/extension/ai/black_board.h"

namespace game {

namespace server {

namespace ai {

const std::string RemoveTargetAction::kTypeName_ = "remove_target";

RemoveTargetAction::RemoveTargetAction() {}
RemoveTargetAction::~RemoveTargetAction() {}

bool RemoveTargetAction::LoadConfigure(TiXmlElement *node) {
  return true;
}

void RemoveTargetAction::OnEnter(BlackBoard *black_board) {
}

RunningStatus::type RemoveTargetAction::OnExecute(BlackBoard *black_board) {
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

void RemoveTargetAction::OnQuit(BlackBoard *black_board) {
}

}  // namespace ai

}  // namespace server

}  // namespace game

