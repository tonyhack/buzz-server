//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-22 09:32:34.
// File name: playing_load_balance.h
//
// Description:
// Define class PlayingLoadBalance.
//

#ifndef __SESSION__SERVER__PLAYING__PLAYING__LOAD__BANLANCE__H
#define __SESSION__SERVER__PLAYING__PLAYING__LOAD__BANLANCE__H

#include <vector>

#include "core/base/types.h"
#include "global/singleton_factory.h"

namespace session {

namespace server {

namespace playing {

class PlayingLoadBalance : public global::SingletonFactory<PlayingLoadBalance> {
  friend class global::SingletonFactory<PlayingLoadBalance>;

  typedef std::pair<core::uint32, core::uint32> PlayingPair;
  typedef std::vector<PlayingPair> PlayingPairVector;

 public:
  core::uint32 Allocate();
  void Deallocate(core::uint32 server_id);

  void OnGameServerLogin(core::uint32 server_id);
  void OnGameServerLogout(core::uint32 server_id);

 private:
  PlayingLoadBalance() {}
  ~PlayingLoadBalance() {}

  PlayingPairVector playings_;
};

}  // namespace playing

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__PLAYING__PLAYING__LOAD__BANLANCE__H

