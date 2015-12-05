//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-16 10:00:17.
// File name: playing_page_pool.h
//
// Description:
// Define class PlayingPagePool.
//

#ifndef __GAME__SERVER__PLAYING__PLAYING__PAGE__POOL__H
#define __GAME__SERVER__PLAYING__PLAYING__PAGE__POOL__H

#include "global/pool_template.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace playing {

class PlayingPage;

class PlayingPagePool : public global::SingletonFactory<PlayingPagePool> {
  friend class global::SingletonFactory<PlayingPagePool>;

 public:
  bool Initialize(size_t initial_number, size_t extend_number);
  void Finalize();

  PlayingPage *Allocate();
  void Deallocate(PlayingPage *playing);

 private:
  PlayingPagePool() {}
  ~PlayingPagePool() {}

  global::PoolTemplate<PlayingPage> pages_;
};

}  // namespace playing

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__PLAYING__PLAYING__PAGE__POOL__H

