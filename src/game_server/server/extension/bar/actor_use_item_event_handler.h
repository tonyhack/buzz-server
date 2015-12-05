//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-11-09 19:49:13.
// File name: actor_use_item_event_handler.h
//
// Description:
//

#ifndef __GAME__SERVER__BAR__ACTOR__USE__ITEM__EVENT__HANDLER__H
#define __GAME__SERVER__BAR__ACTOR__USE__ITEM__EVENT__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace bar {

class ActorUseItemEventHandler {
 public:
  ActorUseItemEventHandler();
  ~ActorUseItemEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnEvent(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection conn_use_item_;

};

} // namespace bar

} // namepsace server

} // namepsace game

#endif  // __GAME__SERVER__BAR__ACTOR__USE__ITEM__EVENT__HANDLER__H
