//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-10-17 15:31:58.
// File name: request_add_threat.h
//
// Description:
// Define class RequestAddThreat.
//

#ifndef __GAME__SERVER__AI__ADD__THREAT__H
#define __GAME__SERVER__AI__ADD__THREAT__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace ai {

class RequestAddThreat : public core::Noncopyable {
 public:
  RequestAddThreat();
  ~RequestAddThreat();

  bool Initialize();
  void Finalize();

 private:
  int OnRequest(void *message, size_t size);
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__ADD__THREAT__H

