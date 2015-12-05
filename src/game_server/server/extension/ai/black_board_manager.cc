//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-09 19:26:47.
// File name: black_board_manager.cc
//
// Description:
// Define class BlackBoardManager.
//

#include "game_server/server/extension/ai/black_board_manager.h"

#include "entity/entity_types.h"
#include "game_server/server/extension/ai/black_board.h"
#include "game_server/server/extension/ai/black_board_pool.h"

namespace game {

namespace server {

namespace ai {

bool BlackBoardManager::Initialize() {
  return true;
}

void BlackBoardManager::Finalize() {
  BlackBoardHashmap::iterator iterator = this->black_boards_.begin();
  for(; iterator != this->black_boards_.end(); ++iterator) {
    if(iterator->second) {
      iterator->second->Finalize();
      BlackBoardPool::GetInstance()->Deallocate(iterator->second);
    }
  }
  this->black_boards_.clear();
}

BlackBoard *BlackBoardManager::GetNpc(core::uint64 id) {
  BlackBoardHashmap::iterator iterator = this->black_boards_.find(id);
  if(iterator != this->black_boards_.end()) {
    return iterator->second;
  }
  return NULL;
}

bool BlackBoardManager::AddNpc(BlackBoard *black_board) {
  // 检测指针是否合法
  if(black_board == NULL) {
    return false;
  }
  // 检测类型是否正确
  if(black_board->GetType() != entity::EntityType::TYPE_NPC) {
    return false;
  }
  // 检测是否存在
  BlackBoardHashmap::iterator iterator = this->black_boards_.find(black_board->GetID());
  if(iterator != this->black_boards_.end()) {
    return false;;
  }
  // 加入管理器
  this->black_boards_.insert(std::make_pair(black_board->GetID(), black_board));
  return true;
}

BlackBoard *BlackBoardManager::RemoveNpc(core::uint64 id) {
  BlackBoard *black_board = NULL;
  BlackBoardHashmap::iterator iterator = this->black_boards_.find(id);
  if(iterator != this->black_boards_.end()) {
    black_board = iterator->second;
    this->black_boards_.erase(iterator);
  }
  return black_board;
}

}  // namespace ai

}  // namespace server

}  // namespace game

