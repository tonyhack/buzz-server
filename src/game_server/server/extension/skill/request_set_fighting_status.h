//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-06-25 17:45:39.
// File name: request_set_fighting_status.h
//
// Description:
// Define class RequestSetFightingStatus.
//

#ifndef __GAME__SERVER__SKILL__REQUEST__SET__FIGHTING__STATUS__H
#define __GAME__SERVER__SKILL__REQUEST__SET__FIGHTING__STATUS__H

#include <cstddef>

#include "core/base/noncopyable.h"

namespace game {

namespace server {

namespace skill {

class RequestSetFightingStatus : public core::Noncopyable {
 public:
  RequestSetFightingStatus();
  ~RequestSetFightingStatus();

  bool Initialize();
  void Finalize();

 private:
  int OnRequest(void *message, size_t size);
};

}  // namespace skill

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SKILL__REQUEST__SET__FIGHTING__STATUS__H

