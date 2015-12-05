#ifndef __GAME__SERVER__MAIL__MAIL__POOL__H
#define __GAME__SERVER__MAIL__MAIL__POOL__H

#include "global/pool_template.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace mail {

class Mail;

class MailPool : public global::SingletonFactory<MailPool> {
  friend class global::SingletonFactory<MailPool>;
 public:
  bool Initialize(size_t initial_number, size_t extend_number);
  void Finalize();

  Mail *Allocate();
  void Deallocate(Mail *p_mail);

 private:
  MailPool();
  ~MailPool();

  global::PoolTemplate<Mail> mails_;
};

}  // namespace mail

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__MAIL__MAIL__ACTOR__POOL__H

