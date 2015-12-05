#ifndef __GAME__SERVER__WELFARE__ROLE__DIE__EVENT__HANDLER__H
#define __GAME__SERVER__WELFARE__ROLE__DIE__EVENT__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace welfare {

class RoleDieEventHandler {
 public:
  RoleDieEventHandler();
  ~RoleDieEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnEventNpc(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  void OnEventActor(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection conn_npc_killed_;
  coresh::Communicator::Connection conn_actor_killed_;

};

} // namespace welfare

} // namepsace server

} // namepsace game

#endif  // __GAME__SERVER__WELFARE__ROLE__DIE__EVENT__HANDLER__H
