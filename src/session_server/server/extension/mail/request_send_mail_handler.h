//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-09-30 03:36:39.
// File name: requestMailSend.h
//
// Description:
//
#ifndef __SESSION__SERVER__REQUEST__SEND__MAIL__HANDLER__H
#define __SESSION__SERVER__REQUEST__SEND__MAIL__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"

namespace session {

namespace server {

namespace mail {

class RequestSendMailHandler : public core::Noncopyable {
 public:
  RequestSendMailHandler();
  ~RequestSendMailHandler();

  bool Initialize();
  void Finalize();

 private:
  int OnRequestSendMail(void *message, size_t size);
};

}  // namespace mail

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__REQUEST__SEND__MAIL__HANDLER__H

