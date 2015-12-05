//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-09 19:21:57.
// File name: black_board_manager.h
//
// Description:
// Define class BlackBoardManager.
//

#ifndef __GAME__SERVER__AI__BLACK__BOARD__MANAGER__H
#define __GAME__SERVER__AI__BLACK__BOARD__MANAGER__H

#include <ext/hash_map>

#include "core/base/types.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace ai {

class BlackBoard;

class BlackBoardManager : public global::SingletonFactory<BlackBoardManager> {
  friend class global::SingletonFactory<BlackBoardManager>;

  typedef __gnu_cxx::hash_map<core::uint64, BlackBoard *> BlackBoardHashmap;

 public:
  bool Initialize();
  void Finalize();

  BlackBoard *GetNpc(core::uint64 id);
  bool AddNpc(BlackBoard *black_board);
  BlackBoard *RemoveNpc(core::uint64 id);

 private:
  BlackBoardManager() {}
  ~BlackBoardManager() {}

  BlackBoardHashmap black_boards_;
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__BLACK__BOARD__MANAGER__H

