//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-27 18:01:29.
// File name: request_check_in_cooling.h
//
// Description:
// Define class RequestCheckInCooling.
//

#ifndef __GAME__SERVER__COOLING__REQUEST__CHECK__IN__COOLING__H
#define __GAME__SERVER__COOLING__REQUEST__CHECK__IN__COOLING__H

#include <stddef.h>

#include "core/base/noncopyable.h"

namespace game {

namespace server {

namespace cooling {

class RequestCheckInCooling : public core::Noncopyable {
 public:
  RequestCheckInCooling();
  ~RequestCheckInCooling();

  bool Initialize();
  void Finalize();

 private:
  int OnRequest(void *message, size_t size);
};

}  // namespace cooling

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__COOLING__REQUEST__CHECK__IN__COOLING__H

