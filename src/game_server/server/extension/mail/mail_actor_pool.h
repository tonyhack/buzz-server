//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-09-22 18:12:28.
// File name: mail_actor_pool.h
//
// Description:
//

#ifndef __GAME__SERVER__MAIL__MAIL__ACTOR__POOL__H
#define __GAME__SERVER__MAIL__MAIL__ACTOR__POOL__H

#include "game_server/server/extension/mail/mail_actor.h"
#include "global/pool_template.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace mail {

class MailActor;

class MailActorPool : public global::SingletonFactory<MailActorPool> {
  friend class global::SingletonFactory<MailActorPool>;

 public:
  bool Initialize(size_t initial_number, size_t extend_number);
  void Finalize();

  MailActor *Allocate();
  void Deallocate(MailActor *mail_actor);

 private:
  MailActorPool();
  ~MailActorPool();

  global::PoolTemplate<MailActor> mail_actors_;
};

}  // namespace mail

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__MAIL__MAIL__ACTOR__POOL__H
