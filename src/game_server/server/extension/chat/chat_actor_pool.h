#ifndef __GAME__SERVER__CHAT__CHAT__ACTOR__POOL__H
#define __GAME__SERVER__CHAT__CHAT__ACTOR__POOL__H

#include "global/pool_template.h"
#include "global/singleton_factory.h"
#include "game_server/server/extension/chat/chat_actor.h"

namespace game {

namespace server {

namespace chat {

class ChatActorPool : public global::SingletonFactory<ChatActorPool> {
  friend class global::SingletonFactory<ChatActorPool>;

 public:
  bool Initialize(size_t initial_number, size_t extend_number);
  void Finalize();

  ChatActor *Allocate();
  void Deallocate(ChatActor *actor);

 private:
  ChatActorPool() {}
  ~ChatActorPool() {}

  global::PoolTemplate<ChatActor> actors_;
};

}  // namespace chat

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__CHAT__CHAT__ACTOR__POOL__H

