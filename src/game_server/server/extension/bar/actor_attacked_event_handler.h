//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-11-08 23:31:36.
// File name: actor_attacked_event_handler.h
//
// Description:
//

#ifndef __GAME__SERVER__BAR__ACTOR__ATTACKED__EVENT__HANDLER__H
#define __GAME__SERVER__BAR__ACTOR__ATTACKED__EVENT__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace bar {

class ActorAttackedEventHandler {
 public:
  ActorAttackedEventHandler();
  ~ActorAttackedEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnEvent(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection conn_hurt_;

};

} // namespace bar

} // namepsace server

} // namepsace game

#endif  // __GAME__SERVER__BAR__ACTOR__ATTACKED__EVENT__HANDLER__H
