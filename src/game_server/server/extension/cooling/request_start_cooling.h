//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-27 18:48:49.
// File name: request_start_cooling.h
//
// Description:
// Define class RequestStartCooling.
//

#ifndef __GAME__SERVER__COOLING__REQUEST__START__COOLING__H
#define __GAME__SERVER__COOLING__REQUEST__START__COOLING__H

#include <stddef.h>

#include "core/base/noncopyable.h"

namespace game {

namespace server {

namespace cooling {

class RequestStartCooling : public core::Noncopyable {
 public:
  RequestStartCooling();
  ~RequestStartCooling();

  bool Initialize();
  void Finalize();

 private:
  int OnRequest(void *message, size_t size);
};

}  // namespace cooling

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__COOLING__REQUEST__START__COOLING__H

