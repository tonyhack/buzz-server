//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-10-24 00:54:36.
// File name: actor_event_handler.h
//
// Description:
//
#ifndef __GAME__SERVER__TASK__ACTOR__ATTR__EVENT__HANDLER__H
#define __GAME__SERVER__TASK__ACTOR__ATTR__EVENT__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace task {

class ActorAttrEventHandler {
 public:
  ActorAttrEventHandler();
  ~ActorAttrEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnEventActorLevelUp(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  void OnEventActorResourceChange(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection conn_level_up_;
  coresh::Communicator::Connection conn_resource_change_;
};

} // namespace task

} // namepsace server

} // namepsace game

#endif  // __GAME__SERVER__TASK__ACTOR__ATTR__EVENT__HANDLER__H
