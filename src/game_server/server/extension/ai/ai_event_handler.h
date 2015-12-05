//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Eai: lxf@gmail.com.
// Last modify: 2013-09-26 20:00:44.
// File name: ai_event_handler.h
//
// Description:
//
#ifndef __GAME__SERVER__AI__AI__EVENT__HANDLER__H
#define __GAME__SERVER__AI__AI__EVENT__HANDLER__H

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace ai {

class AiEventHandler : public core::Noncopyable {
 public:
  AiEventHandler();
  ~AiEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnLoginEvent(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  void OnLogoutEvent(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection conn_actor_login_;
  coresh::Communicator::Connection conn_actor_logout_;
};

}  // namespace ai

}  // namespace server

}  // namespace game


#endif  // __GAME__SERVER__AI__AI__EVENT__HANDLE__H

