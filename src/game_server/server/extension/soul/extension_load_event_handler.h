//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-17 11:33:17.
// File name: extension_load_event_handler.h
//
// Description:
// Define class ExtensionLoadEventHandler.
//

#ifndef __GAME__SERVER__SOUL__EXTENSION__LOAD__EVENT__HANDLER__H
#define __GAME__SERVER__SOUL__EXTENSION__LOAD__EVENT__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace soul {

class ExtensionLoadEventHandler : public core::Noncopyable {
 public:
  ExtensionLoadEventHandler();
  ~ExtensionLoadEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnEventActorExtensionLoadFinish(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnEventActorLoadFinish(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection conn_extension_load_;
  coresh::Communicator::Connection conn_actor_load_;
};

}  // namespace soul

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SOUL__EXTENSION__LOAD__EVENT__HANDLER__H

