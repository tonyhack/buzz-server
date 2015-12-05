//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-21 12:16:09.
// File name: request_select_targets.h
//
// Description:
// Define class RequestSelectTargets.
//

#ifndef __GAME__SERVER__SCENE__REQUEST__SELECT__TARGETS__H
#define __GAME__SERVER__SCENE__REQUEST__SELECT__TARGETS__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace scene {

class RequestSelectTargets : public core::Noncopyable {
 public:
  RequestSelectTargets();
  ~RequestSelectTargets();

  bool Initialize();
  void Finalize();

 private:
  int OnRequest(void *message, size_t size);
};

}  // namespace scene

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SCENE__REQUEST__SELECT__TARGETS__H

