#ifndef __GAME__SERVER__WELFARE__REQUEST__HANDLER__H
#define __GAME__SERVER__WELFARE__REQUEST__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"

namespace game {

namespace server {

namespace welfare {

class RequestHandler : public core::Noncopyable {
 public:
  RequestHandler();
  ~RequestHandler();

  bool Initialize();
  void Finalize();

 private:
  int OnGetTitleIDRequest(void *message, size_t size);
};

}  // namespace welfare

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__WELFARE__REQUEST__HANDLER__H

