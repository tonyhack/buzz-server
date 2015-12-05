//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-11-26 12:00:42.
// File name: buff_event_handler.h
//
// Description:
// Define class BuffEventHandler.
//

#ifndef __GAME__SERVER__BUFF__BUFF__EVENT__HANDLER__H
#define __GAME__SERVER__BUFF__BUFF__EVENT__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace buff {

class BuffEventHandler : public core::Noncopyable {
 public:
  BuffEventHandler();
  ~BuffEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnEventBuffStart(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnEventBuffEnd(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection conn_npc_buff_start_;
  coresh::Communicator::Connection conn_actor_buff_start_;

  coresh::Communicator::Connection conn_npc_buff_end_;
  coresh::Communicator::Connection conn_actor_buff_end_;
};

}  // namespace buff

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__BUFF__BUFF__EVENT__HANDLER__H

