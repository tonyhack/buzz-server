//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-03-31 19:44:28.
// File name: extension_load_event_handler.h
//
// Description:
// Define class ExtensionLoadEventHandler.
//

#ifndef __GAME__SERVER__EFFECT__EXTENSION__LOAD__EVENT__HANDLER__H
#define __GAME__SERVER__EFFECT__EXTENSION__LOAD__EVENT__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace effect {

class ExtensionLoadEventHandler : public core::Noncopyable {
 public:
  ExtensionLoadEventHandler();
  ~ExtensionLoadEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnEvent(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection connection_;
};

}  // namespace effect

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__EFFECT__EXTENSION__LOAD__EVENT__HANDLER__H

