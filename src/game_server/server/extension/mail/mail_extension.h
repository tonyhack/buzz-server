#ifndef __GAME__SERVER__MAIL__MAIL__EXTENSION__H
#define __GAME__SERVER__MAIL__MAIL__EXTENSION__H

#include "game_server/server/extension/mail/mail_request.h"
#include "game_server/server/extension/mail/mail_database_response.h"
#include "game_server/server/extension/mail/mail_event_handler.h"
#include "game_server/server/extension/mail/mail_message_handler.h"
#include "game_server/server/extension/mail/session_message_handler.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace mail {

class MailExtension : public global::SingletonFactory<MailExtension> {
 friend class global::SingletonFactory<MailExtension>;
 public:
  MailExtension();
  ~MailExtension();

  bool Initialize();
  void Finalize();

  void Start();
  void Stop();
  
 private:
  // 数据库回调
  MailDatabaseResponse response_;
  // 事件回调
  MailEventHandler event_;
  // 消息回调
  MailMessageHandler message_;
  // 处理 session 消息 
  SessionMessageHandler session_message_;
  // 请求回调
  RequestMailHandler request_;
};

}  // namespace mail

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__MAIL__MAIL__EXTENSION__H
