//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-10 09:52:38.
// File name: ai_controller.cc
//
// Description:
// Define class AiController.
//

#include "game_server/server/extension/ai/ai_controller.h"

#include "game_server/server/game_server.h"
#include "game_server/server/extension/ai/black_board.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

bool AiController::Initialize() {
  this->ai_running_ = false;
  return true;
}

void AiController::Finalize() {
}

void AiController::Start() {
  GameServerSingleton::GetInstance().AddTimer(0, 50, -1, this,
      "AiController::Initialize");
}

void AiController::Stop() {
  GameServerSingleton::GetInstance().RemoveTimer(0, this);
}

bool AiController::AddBlackBoard(BlackBoard *black_board) {
  if(black_board == NULL) {
    global::LogError("%s:%d (%s) 参数 black_board 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(black_board->CheckHasAi()) {
    if(this->ai_running_ == true) {
      this->adds_.push_back(std::make_pair(black_board, true));
    } else {
      this->black_boards_.insert(black_board);
      black_board->OnEnter();
    }
  }
  return true;
}

void AiController::RemoveBlackBoard(BlackBoard *black_board) {
  if(this->ai_running_ == true) {
    this->adds_.push_back(std::make_pair(black_board, false));
  } else {
    this->black_boards_.erase(black_board);
    black_board->OnQuit();
  }
}

void AiController::OnTimer(core::uint32 id) {
  this->OnRunning();
}

void AiController::OnRunning() {
  // 添加/删除
  BlackBoardVector::iterator iterator1 = this->adds_.begin();
  for(; iterator1 != this->adds_.end(); ++iterator1) {
    if(iterator1->second == true) {
      this->black_boards_.insert(iterator1->first);
      iterator1->first->OnEnter();
    } else {
      this->black_boards_.erase(iterator1->first);
      iterator1->first->OnQuit();
    }
  }
  this->adds_.clear();

  this->ai_running_ = true;
  // Ai运行
  BlackBoardSet::iterator iterator = this->black_boards_.begin();
  for(; iterator != this->black_boards_.end(); ++iterator) {
    (*iterator)->Running();
  }
  this->ai_running_ = false;
}

}  // namespace ai

}  // namespace server

}  // namespace game

