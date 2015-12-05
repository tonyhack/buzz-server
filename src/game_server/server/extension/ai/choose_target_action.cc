//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-17 17:56:39.
// File name: choose_target_action.cc
//
// Description:
// Define class ChooseTargetAction.
//

#include "game_server/server/extension/ai/choose_target_action.h"

#include "game_server/server/extension/ai/black_board.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

const std::string ChooseTargetAction::kTypeName_ = "choose_target";

ChooseTargetAction::ChooseTargetAction() {}
ChooseTargetAction::~ChooseTargetAction() {}

bool ChooseTargetAction::LoadConfigure(TiXmlElement *node) {
  return true;
}

void ChooseTargetAction::OnEnter(BlackBoard *black_board) {}

RunningStatus::type ChooseTargetAction::OnExecute(BlackBoard *black_board) {
  if(black_board == NULL) {
    return RunningStatus::ERROR;
  }

  core::uint64 max_thread_actor = black_board->GetThreat().GetMaxThreatActor();
  black_board->SetTarget(entity::EntityType::TYPE_ACTOR, max_thread_actor);

  return RunningStatus::FINISH;
}

void ChooseTargetAction::OnQuit(BlackBoard *black_board) {}

}  // namespace ai

}  // namespace server

}  // namespace game

