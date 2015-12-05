#ifndef __GAME__SERVER__MAIL__MAIL__ACTOR__H
#define __GAME__SERVER__MAIL__MAIL__ACTOR__H

#include <map>

#include "core/base/noncopyable.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension/mail/mail.h"
#include "game_server/server/game_actor.h"

namespace game {

namespace server {

namespace mail {

class Mail;

class MailActor : public core::Noncopyable {
  typedef std::map<core::int32, Mail *> MailMap;
 public:
  MailActor();
  ~MailActor();

  bool Initialize(GameActor *actor);
  void Finalize();   
  // 查找邮件
  Mail *GetMail(core::int32 id);
  // 删除邮件
  bool EraseMail(core::int32 id);
  // 添加邮件
  bool AddMail(Mail* mail);
  // 得到玩家ID
  inline core::uint64 GetID() const {
    if(this->game_actor_) {
      return this->game_actor_->GetID();
    }
    return 0;
  }
  // 得到玩家
  GameActor *GetActor() const {
    return game_actor_;
  }
 private:
  // 是否加载完毕
  bool load_finish_;

  GameActor *game_actor_; 
  // 邮件
  MailMap mails_;
};

}  // namespace mail

}  // namespace server

}  // namespace game

#endif // __GAME__SERVER__MAIL__MAIL__ACTOR__H

