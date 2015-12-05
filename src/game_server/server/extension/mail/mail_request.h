//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-11-04 03:11:48.
// File name: mail_request.h
//
// Description:
//
#ifndef __GAME__SERVER__MAIL__MAIL__REQUEST__H
#define __GAME__SERVER__MAIL__MAIL__REQUEST__H

#include <stddef.h>

#include "core/base/noncopyable.h"

namespace game {

namespace server {

namespace mail {

class RequestMailHandler : public core::Noncopyable {
 public:
  RequestMailHandler();
  ~RequestMailHandler();

  bool Initialize();
  void Finalize();

 private:
  int OnRequestMailSend(void *message, size_t size);
};

}  // namespace mail

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__MAIL__MAIL__REQUEST__H

