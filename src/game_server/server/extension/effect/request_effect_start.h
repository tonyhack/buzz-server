//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-04 00:07:29.
// File name: request_effect_start.h
//
// Description:
// Define class RequestEffectStart.
//

#ifndef __GAME__SERVER__EFFECT__REQUEST__EFFECT__START__H
#define __GAME__SERVER__EFFECT__REQUEST__EFFECT__START__H

#include <stddef.h>

#include "core/base/noncopyable.h"

namespace game {

namespace server {

namespace effect {

class RequestEffectStart : public core::Noncopyable {
 public:
  RequestEffectStart();
  ~RequestEffectStart();

  bool Initialize();
  void Finalize();

 private:
  int OnRequestEffectStart(void *message, size_t size);
  int OnRequestCommonStatusChange(void *message, size_t size);
};

}  // namespace effect

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__EFFECT__REQUEST__EFFECT__START__H

