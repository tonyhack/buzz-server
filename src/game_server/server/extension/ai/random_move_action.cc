//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-16 13:31:43.
// File name: random_move_action.cc
//
// Description:
// Define class RandomMoveAction.
//

#include "game_server/server/extension/ai/random_move_action.h"

#include "game_server/server/extension/ai/black_board.h"
#include "global/logging.h"
#include "global/types.h"

namespace game {

namespace server {

namespace ai {

const std::string RandomMoveAction::kTypeName_ = "random_move";

RandomMoveAction::RandomMoveAction() {}
RandomMoveAction::~RandomMoveAction() {}

bool RandomMoveAction::LoadConfigure(TiXmlElement *node) {
  if(node == NULL) {
    global::LogError("%s:%d (%s) 加载AI结点失败，参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(node->Attribute("param1", (int *)(&this->odds_)) == false) {
    global::LogError("%s:%d (%s) 找不到结点[node-param1]",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(node->Attribute("param2", (int *)(&this->move_interval_)) == false) {
    global::LogError("%s:%d (%s) 找不到结点[node-param2]",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  return true;
}

void RandomMoveAction::OnEnter(BlackBoard *black_board) {}

RunningStatus::type RandomMoveAction::OnExecute(BlackBoard *black_board) {
  if(black_board == NULL) {
    return RunningStatus::ERROR;
  }

/*
  // 移动频率检测
  if(black_board->CheckMove() == false) {
    return RunningStatus::EXECUTING;
  }
*/
  // 随机移动频率检测
  if(black_board->CheckRandomMove(this->move_interval_, true) == false) {
    return RunningStatus::EXECUTING;
  }

  // 随机
  core::uint32 random =
    ExtensionManager::GetInstance()->GetRandom().Random(global::kPercentDenom);
  if(this->odds_ >= random) {
    black_board->Move(entity::DirectionType::NONE);
  }

  return RunningStatus::EXECUTING;
}

void RandomMoveAction::OnQuit(BlackBoard *black_board) {
}

}  // namespace ai

}  // namespace server

}  // namespace game

