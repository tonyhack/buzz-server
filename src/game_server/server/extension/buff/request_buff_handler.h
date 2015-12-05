//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-04 00:07:29.
// File name: request_buff_handler.h
//
// Description:
// Define class RequestBuffHandler.
//

#ifndef __GAME__SERVER__BUFF__REQUEST__BUFF__HANDLER__H
#define __GAME__SERVER__BUFF__REQUEST__BUFF__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"

namespace game {

namespace server {

namespace buff {

class RequestBuffHandler : public core::Noncopyable {
 public:
  RequestBuffHandler();
  ~RequestBuffHandler();

  bool Initialize();
  void Finalize();

 private:
  int OnRequestBuffStart(void *message, size_t size);
  int OnRequestBuffEnd(void *message, size_t size);
  int OnRequestBuffClear(void *message, size_t size);
  int OnRequestBuffClearGroup(void *message, size_t size);
  int OnRequestBuffSetForbidGroup(void *message, size_t size);
  int OnRequestBuffResetForbidGroup(void *message, size_t size);
  int OnRequestBuffGetExist(void *message, size_t size);
};

}  // namespace buff

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__BUFF__REQUEST__BUFF__HANDLER__H

