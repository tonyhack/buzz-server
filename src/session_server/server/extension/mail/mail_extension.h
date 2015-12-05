//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-09-19 00:19:52.
// File name: mail_extension.h
//
// Description:
//
#ifndef __SESSION__SERVER__MAIL__MAIL__EXTENSION__H
#define __SESSION__SERVER__MAIL__MAIL__EXTENSION__H

#include "core/base/noncopyable.h"
#include "session_server/server/extension/mail/game_message_handler.h"
#include "session_server/server/extension/mail/storage_mail_response.h"
#include "session_server/server/extension/mail/request_send_mail_handler.h"

namespace session {

namespace server {

namespace mail {

class MailExtension : public core::Noncopyable {
 public:
  MailExtension();
  ~MailExtension();

  bool Initialize();
  void Finalize();

  void Start();
  void Stop();

 private:
  GameMessageHandler game_message_handler_;
  StorageMailResponse response_;
  RequestSendMailHandler request_send_;
};

} // namespace mail

} // namespace server

} // namespace session

#endif // __SESSION__SERVER__MAIL__MAIL__EXTENSION__H

