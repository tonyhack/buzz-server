#ifndef __GAME__SERVER__CHAT__CHAT__ACTOR__MANAGER__H
#define __GAME__SERVER__CHAT__CHAT__ACTOR__MANAGER__H

#include <ext/hash_map>

#include "core/base/types.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace chat {

class ChatActor;

class ChatActorManager : public global::SingletonFactory<ChatActorManager> {
  friend class global::SingletonFactory<ChatActorManager>;

  typedef __gnu_cxx::hash_map<core::uint64, ChatActor *> ActorHashmap;

 public:
  ChatActor *Get(core::uint64 id);
  bool Add(ChatActor *actor);
  ChatActor *Remove(core::uint64 id);

 private:
  ChatActorManager() {}
  ~ChatActorManager() {}

  ActorHashmap actors_;
};

}  // namespace chat

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__CHAT__CHAT__ACTOR__MANAGER__H

