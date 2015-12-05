//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-16 10:00:17.
// File name: playing_pool.h
//
// Description:
// Define class PlayingPool.
//

#ifndef __SESSION__SERVER__PLAYING__PLAYING__POOL__H
#define __SESSION__SERVER__PLAYING__PLAYING__POOL__H

#include "core/base/noncopyable.h"
#include "global/pool_template.h"
#include "global/singleton_factory.h"

namespace session {

namespace server {

namespace playing {

class Playing;

class PlayingPool : public global::SingletonFactory<PlayingPool> {
  friend class global::SingletonFactory<PlayingPool>;

 public:
  bool Initialize(size_t initial_number, size_t extend_number);
  void Finalize();

  Playing *Allocate();
  void Deallocate(Playing *playing);

 private:
  PlayingPool() {}
  ~PlayingPool() {}

  global::PoolTemplate<Playing> playings_;

  core::uint64 id_;
};

}  // namespace playing

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__PLAYING__PLAYING__POOL__H

