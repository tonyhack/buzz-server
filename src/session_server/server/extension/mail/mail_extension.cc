//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-09-19 00:34:26.
// File name: mail_extension.cc
//
// Description:
//
#include "session_server/server/extension/mail/mail_extension.h"

namespace session {

namespace server {

namespace mail {

MailExtension::MailExtension() {}
MailExtension::~MailExtension() {}

bool MailExtension::Initialize() {
  if(game_message_handler_.Initialize() == false) {
    LOG_ERROR("Initialize GameMessageHandler failed.");
    return false;
  }

  if(response_.Initialize() == false) {
    LOG_ERROR("Initialize StorageMailResponse failed.");
    return false;
  }
  if(request_send_.Initialize() == false) {
    LOG_ERROR("Initialize RequestSendMailHandler failed.");
    return false;
  }
  return true;
}

void MailExtension::Finalize() {
   request_send_.Finalize();
   response_.Finalize();
   game_message_handler_.Finalize();
}

void MailExtension::Start() {}

void MailExtension::Stop() {}

} // namespace mail

} // namepsace server

} // namespace session

