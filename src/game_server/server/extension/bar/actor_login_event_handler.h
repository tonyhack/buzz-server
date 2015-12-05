//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-11-03 20:11:40.
// File name: actor_login_event.h
//
// Description:
//

#ifndef __GAME__SERVER__BAR__ACTOR__LOGIN__EVENT__HANDLER__H
#define __GAME__SERVER__BAR__ACTOR__LOGIN__EVENT__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace bar {

class ActorLoginEventHandler : public core::Noncopyable {
 public:
  ActorLoginEventHandler();
  ~ActorLoginEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnEvent(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection conn_actor_login_;
};

}  // namespace bar

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__BAR__ACTOR__LOGIN__EVENT__HANDLER__H

