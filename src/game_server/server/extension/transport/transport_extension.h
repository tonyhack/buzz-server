//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-06-08 23:25:28.
// File name: transport_extension.h
//
// Description:
// Define class TransportExtension.
//

#ifndef __GAME__SERVER__TRANSPORT__TRANSPORT__EXTENSION__H
#define __GAME__SERVER__TRANSPORT__TRANSPORT__EXTENSION__H

#include "core/base/noncopyable.h"
#include "game_server/server/extension/transport/transport_actor_manager.h"
#include "game_server/server/extension/transport/transport_npc_manager.h"
#include "game_server/server/extension/transport/request_transport_handler.h"
#include "game_server/server/extension/transport/transport_event_handler.h"

namespace game {

namespace server {

namespace transport {

class TransportExtension : public global::SingletonFactory<TransportExtension> {
  friend class global::SingletonFactory<TransportExtension>;

 public:
  bool Initialize();
  void Finalize();

  void Start();
  void Stop();

 private:
  TransportExtension() {}
  ~TransportExtension() {}

  RequestTransportHandler request_handler_;
  TransportEventHandler event_handler_;
};

}  // namespace transport

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TRANSPORT__TRANSPORT__EXTENSION__H

