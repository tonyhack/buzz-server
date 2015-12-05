//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-16 09:32:44.
// File name: playing_actor_pool.h
//
// Description:
// Define class PlayingActorPool.
//

#ifndef __SESSION__SERVER__PLAYING__PLAYING__ACTOR__POOL__H
#define __SESSION__SERVER__PLAYING__PLAYING__ACTOR__POOL__H

#include "core/base/noncopyable.h"
#include "global/pool_template.h"
#include "global/singleton_factory.h"

namespace session {

namespace server {

namespace playing {

class PlayingActor;

class PlayingActorPool : public global::SingletonFactory<PlayingActorPool> {
  friend class global::SingletonFactory<PlayingActorPool>;

 public:
  bool Initialize(size_t initial_number, size_t extend_number);
  void Finalize();

  PlayingActor *Allocate();
  void Deallocate(PlayingActor *actor);

 private:
  PlayingActorPool() {}
  ~PlayingActorPool() {}

  global::PoolTemplate<PlayingActor> actors_;
};

}  // namespace playing

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__PLAYING__PLAYING__ACTOR__POOL__H

