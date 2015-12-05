//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-09 19:18:04.
// File name: black_board_pool.cc
//
// Description:
// Define class BlackBoardPool.
//

#include "game_server/server/extension/ai/black_board_pool.h"

#include "game_server/server/extension/ai/black_board.h"

namespace game {

namespace server {

namespace ai {

bool BlackBoardPool::Initialize(size_t initial_number,
    size_t extend_number) {
  return this->black_boards_.Initialize(initial_number, extend_number);
}

void BlackBoardPool::Finalize() {
  this->black_boards_.Finalize();
}

BlackBoard *BlackBoardPool::Allocate() {
  return this->black_boards_.Allocate();
}

void BlackBoardPool::Deallocate(BlackBoard *black_board) {
  this->black_boards_.Deallocate(black_board);
}

}  // namespace ai

}  // namespace server

}  // namespace game

