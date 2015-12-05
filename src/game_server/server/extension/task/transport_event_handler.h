//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-06-13 00:57:34.
// File name: transport_event_handler.h
//
// Description:
// Define class TransportEventHandler.
//

#ifndef __GAME__SERVER__TASK__TRANSPORT__EVENT__HANDLER__H
#define __GAME__SERVER__TASK__TRANSPORT__EVENT__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace task {

class TransportEventHandler : public core::Noncopyable {
 public:
  TransportEventHandler();
  ~TransportEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnEventTransportFailure(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection conn_transport_failure_;
};

}  // namespace task

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TASK__TRANSPORT__EVENT__HANDLER__H

