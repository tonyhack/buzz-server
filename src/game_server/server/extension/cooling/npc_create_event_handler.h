//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-22 14:33:21.
// File name: npc_create_event_handler.h
//
// Description:
// Define class NpcCreateEventHandler.
//

#ifndef __GAME__SERVER__COOLING__NPC__CREATE__EVENT__HANDLER__H
#define __GAME__SERVER__COOLING__NPC__CREATE__EVENT__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace cooling {

class NpcCreateEventHandler : public core::Noncopyable {
 public:
  NpcCreateEventHandler();
  ~NpcCreateEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnEventCreate(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnEventDestory(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection conn_create_;
  coresh::Communicator::Connection conn_destory_;
};

}  // namespace cooling

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__COOLING__NPC__CREATE__EVENT__HANDLER__H

