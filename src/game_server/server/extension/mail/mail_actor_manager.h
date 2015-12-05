#ifndef __GAME__SERVER__MAIL__MAIL__ACTOR__MANAGER__H
#define __GAME__SERVER__MAIL__MAIL__ACTOR__MANAGER__H

#include <ext/hash_map>

#include "core/base/types.h"
#include "global/singleton_factory.h"
#include "game_server/server/extension/mail/mail_actor.h"

namespace game {

namespace server {

namespace mail {

class MailActorManager : public global::SingletonFactory<MailActorManager> {
  friend class global::SingletonFactory<MailActorManager>;

  typedef __gnu_cxx::hash_map<core::uint64, MailActor *> ActorHashmap;

 public:
  MailActor *Get(core::uint64 id);
  MailActor *Remove(core::uint64 id);
  bool Add(MailActor *actor);

 private:
  MailActorManager();
  ~MailActorManager();

  ActorHashmap actors_;
};

}  // namespace mail

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__MAIL__MAIL__ACTOR__MANAGER__H

