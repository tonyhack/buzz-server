//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-06-06 03:00:07.
// File name: request_ai_handler.h
//
// Description:
// Define class RequestAiHandler.
//

#ifndef __GAME__SERVER__AI__REQUEST__AI__HANDLER__H
#define __GAME__SERVER__AI__REQUEST__AI__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace ai {

class RequestAiHandler : public core::Noncopyable {
 public:
  RequestAiHandler();
  ~RequestAiHandler();

  bool Initialize();
  void Finalize();

 private:
  int OnRequestAiSetTarget(void *message, size_t size);
  int OnRequestAiSetAi(void *message, size_t size);
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__REQUEST__AI__HANDLER__H

