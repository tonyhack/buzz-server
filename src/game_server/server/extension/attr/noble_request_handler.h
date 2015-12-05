//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-03-12 02:56:14.
// File name: noble_request_handler.h
//
// Description:
// Define class NobleRequestHandler.
//

#ifndef __GAME__SERVER__ATTR__NOBLE__REQUEST__HANDLER__H
#define __GAME__SERVER__ATTR__NOBLE__REQUEST__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace attr {

class NobleRequestHandler : public core::Noncopyable {
 public:
  NobleRequestHandler();
  ~NobleRequestHandler();

  bool Initialize();
  void Finalize();

 private:
  int OnRequestAttrGetNobleLevel(void *message, size_t size);
};

}  // namespace attr

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ATTR__NOBLE__REQUEST__HANDLER__H

