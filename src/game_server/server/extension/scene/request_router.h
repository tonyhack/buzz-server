//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-27 19:28:58.
// File name: request_router.h
//
// Description:
// Define class RequestRouter.
//

#ifndef __GAME__SERVER__SCENE__REQUEST__ROUTER__H
#define __GAME__SERVER__SCENE__REQUEST__ROUTER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace scene {

class RequestRouter : public core::Noncopyable {
 public:
  RequestRouter();
  ~RequestRouter();

  bool Initialize();
  void Finalize();

 private:
  int OnRequest(void *message, size_t size);
};

}  // namespace scene

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SCENE__REQUEST__ROUTER__H

