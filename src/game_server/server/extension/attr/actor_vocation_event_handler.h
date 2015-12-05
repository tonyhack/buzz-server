//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-11-24 22:14:46.
// File name: actor_vocation_event_handler.h
//
// Description:
//
#ifndef __GAME__SERVER__ATTR__ACTOR__VOCATION__EVENT__HANDLER__H
#define __GAME__SERVER__ATTR__ACTOR__VOCATION__EVENT__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace attr {

class ActorVocationEventHandler : public core::Noncopyable {
 public:
  ActorVocationEventHandler();
  ~ActorVocationEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnEvent(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection conn_actor_;
};

}  // namespace attr

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ATTR__ACTOR__VOCATION__EVENT__HANDLER__H

