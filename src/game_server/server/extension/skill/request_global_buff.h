//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-02-11 03:04:08.
// File name: request_global_buff.h
//
// Description:
// Define class RequestGlobalBuff.
//

#ifndef __GAME__SERVER__SKILL__REQUEST__GLOBAL__BUFF__H
#define __GAME__SERVER__SKILL__REQUEST__GLOBAL__BUFF__H

#include <stddef.h>

#include "core/base/noncopyable.h"

namespace game {

namespace server {

namespace skill {

class RequestGlobalBuff : public core::Noncopyable {
 public:
  RequestGlobalBuff();
  ~RequestGlobalBuff();

  bool Initialize();
  void Finalize();

 private:
  int OnRequestAddGlobalBuff(void *message, size_t size);
  int OnRequestRemoveGlobalBuff(void *message, size_t size);
};

}  // namespace skill

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SKILL__REQUEST__GLOBAL__BUFF__H

