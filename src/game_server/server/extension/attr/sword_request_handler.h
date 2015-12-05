//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-03-12 02:47:28.
// File name: sword_request_handler.h
//
// Description:
// Define class SwordRequestHandler.
//

#ifndef __GAME__SERVER__ATTR__SWORD__REQUEST__HANDLER__H
#define __GAME__SERVER__ATTR__SWORD__REQUEST__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace attr {

class SwordRequestHandler : public core::Noncopyable {
 public: 
  SwordRequestHandler();
  ~SwordRequestHandler();

  bool Initialize();
  void Finalize();

 private:
  int OnRequestAttrGetSwordStep(void *message, size_t size);
};

}  // namespace attr

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ATTR__SWORD__REQUEST__HANDLER__H

