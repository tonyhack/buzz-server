//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-09 18:41:42.
// File name: black_board_pool.h
//
// Description:
// Define class BlackBoardPool.
//

#ifndef __GAME__SERVER__AI__BLACK__BOARD__POOL__H
#define __GAME__SERVER__AI__BLACK__BOARD__POOL__H

#include "global/pool_template.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace ai {

class BlackBoard;

class BlackBoardPool : public global::SingletonFactory<BlackBoardPool> {
  friend class global::SingletonFactory<BlackBoardPool>;

 public:
  bool Initialize(size_t initial_number, size_t extend_number);
  void Finalize();

  BlackBoard *Allocate();
  void Deallocate(BlackBoard *black_board);

 private:
  BlackBoardPool() {}
  ~BlackBoardPool() {}

  global::PoolTemplate<BlackBoard> black_boards_;
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__BLACK__BOARD__POOL__H

