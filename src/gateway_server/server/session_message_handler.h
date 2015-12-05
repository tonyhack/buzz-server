//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-02-26 11:33:44.
// File name: session_message_handler.h
//
// Description:
// Define class SessionMessageHandler.
//

#ifndef __GATEWAY__SERVER__SESSION__MESSAGE__HANDLER__H
#define __GATEWAY__SERVER__SESSION__MESSAGE__HANDLER__H

#include "core/base/noncopyable.h"
#include "global/thrift_packet.h"

namespace gateway {

namespace server {

class SessionMessageHandler : public core::Noncopyable {
 public:
  SessionMessageHandler();
  ~SessionMessageHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnMessage(const char *message, size_t size);
  void OnMessageForward(const char *message, size_t size);
  void OnMessageBroadcast(const char *message, size_t size);
  void OnMessageMulticast(const char *message, size_t size);

  void OnActorLoginResponse(const char *message, size_t size);
  void OnKickoutActor(const char *message, size_t size);
  void OnActorSwitchGame(const char *message, size_t size);

  global::ThriftPacket thrift_;
};

}  // namespace server

}  // namespace gateway

#endif  // __GATEWAY__SERVER__SESSION__MESSAGE__HANDLER__H

