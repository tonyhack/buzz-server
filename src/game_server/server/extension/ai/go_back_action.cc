//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-08 10:38:59.
// File name: go_back_action.cc
//
// Description:
// Define class GoBackAction.
//

#include "game_server/server/extension/ai/go_back_action.h"

#include "game_server/server/game_actor.h"
#include "game_server/server/extension/ai/black_board.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

const std::string GoBackAction::kTypeName_ = "go_back";

GoBackAction::GoBackAction() {}
GoBackAction::~GoBackAction() {}

bool GoBackAction::LoadConfigure(TiXmlElement *node) {
  if(node == NULL) {
    global::LogError("%s:%d (%s) 加载AI结点失败，参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(node->Attribute("param1", (int *)(&this->radius_)) == false) {
    global::LogError("%s:%d (%s) 找不到结点[node-param1]",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  return true;
}

void GoBackAction::OnEnter(BlackBoard *black_board) {
  if(black_board == NULL) {
    return ;
  }

  black_board->GetThreat().RemoveAll();
  black_board->SetTarget(entity::EntityType::TYPE_MAX, 0);
}

RunningStatus::type GoBackAction::OnExecute(BlackBoard *black_board) {
  if(black_board == NULL) {
    return RunningStatus::ERROR;
  }

  if(black_board->CheckMove() == false) {
    return RunningStatus::EXECUTING;
  }

  if(black_board->MoveBack(this->radius_) == false) {
    return RunningStatus::ERROR;
  }

  return RunningStatus::EXECUTING;
}

void GoBackAction::OnQuit(BlackBoard *black_board) {
  if(black_board) {
    black_board->ClearPaths();
  }
}

}  // namespace ai

}  // namespace server

}  // namespace game

