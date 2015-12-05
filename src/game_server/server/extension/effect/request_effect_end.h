//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-07 20:04:16.
// File name: request_effect_end.h
//
// Description:
// Define class RequestEffectEnd.
//

#ifndef __GAME__SERVER__EFFECT__REQUEST__EFFECT__END__H
#define __GAME__SERVER__EFFECT__REQUEST__EFFECT__END__H

#include <stddef.h>

#include "core/base/noncopyable.h"

namespace game {

namespace server {

namespace effect {

class RequestEffectEnd : public core::Noncopyable {
 public:
  RequestEffectEnd();
  ~RequestEffectEnd();

  bool Initialize();
  void Finalize();

 private:
  int OnRequestEffectEnd(void *message, size_t size);
  int OnRequestCommonStatusEnd(void *message, size_t size);
};

}  // namespace effect

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__EFFECT__REQUEST__EFFECT__END__H

