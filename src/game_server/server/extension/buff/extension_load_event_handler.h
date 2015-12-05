//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-22 14:16:34.
// File name: npc_create_event_handler.h
//
// Description:
// Define class NpcCreateEventHandler.
//

#ifndef __GAME__SERVER__BUFF__EXTENSION__LOAD__EVENT__HANDLER__H
#define __GAME__SERVER__BUFF__EXTENSION__LOAD__EVENT__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace buff {

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

}  // namespace buff

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__BUFF__EXTENSION__LOAD__EVENT__SUB__HANDLER__H

