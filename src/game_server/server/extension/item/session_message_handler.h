//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-24 16:02:06.
// File name: session_message_handler.h
//
// Description:
// Define class SessionMessageHandler.
//

#ifndef __GAME__SERVER__ITEM__SESSION__MESSAGE__HANDLER__H
#define __GAME__SERVER__ITEM__SESSION__MESSAGE__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace item {

class SessionMessageHandler : public core::Noncopyable {
 public:
  SessionMessageHandler();
  ~SessionMessageHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnMessageActorCompareSynchronize(const char *data, size_t size);
  void OnMessageActorFacadeSynchronize(const char *data, size_t size);
  void OnMessageItemMallConfigSynchronize(const char *data, size_t size);
};

}  // namespace item

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ITEM__SESSION__MESSAGE__HANDLER__H

