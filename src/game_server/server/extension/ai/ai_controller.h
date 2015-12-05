//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-10 09:45:05.
// File name: ai_controller.h
//
// Description:
// Define class AiController.
//

#ifndef __GAME__SERVER__AI__AI__CONTROLLER__H
#define __GAME__SERVER__AI__AI__CONTROLLER__H

#include <set>
#include <vector>

#include "core/timer_handler.h"
#include "core/base/types.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace ai {

class BlackBoard;

class AiController : public core::TimerHandler,
  public global::SingletonFactory<AiController> {
  friend class global::SingletonFactory<AiController>;

  typedef std::set<BlackBoard *> BlackBoardSet;
  typedef std::pair<BlackBoard *, bool> BlackBoardPair;
  typedef std::vector<BlackBoardPair> BlackBoardVector;

 public:
  bool Initialize();
  void Finalize();

  void Start();
  void Stop();

  // 添加/删除 BlackBoard
  bool AddBlackBoard(BlackBoard *black_board);
  void RemoveBlackBoard(BlackBoard *black_board);

  virtual void OnTimer(core::uint32 id);

 private:
  AiController() {}
  virtual ~AiController() {}

  void OnRunning();

  BlackBoardSet black_boards_;

  BlackBoardVector adds_;

  bool ai_running_;
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__AI__CONTROLLER__H

