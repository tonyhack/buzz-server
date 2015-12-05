//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-02-25 15:30:05.
// File name: gateway_message_handler.h
//
// Description:
// Define class GatewayMessageHandler.
//

#ifndef __SESSION__SERVER__GATEWAY__MESSAGE__HANDLER__H
#define __SESSION__SERVER__GATEWAY__MESSAGE__HANDLER__H

#include "core/base/noncopyable.h"
#include "global/logging.h"
#include "global/singleton_factory.h"
#include "session_server/protocol/session_gateway_protocol_types.h"
#include "session_server/server/session_server.h"

namespace session {

namespace server {

class SessionTerminal;

class GatewayMessageHandler : public global::SingletonFactory<GatewayMessageHandler> {
  friend class global::SingletonFactory<GatewayMessageHandler>;

 public:
  typedef boost::function<void (SessionTerminal *terminal, const char *, size_t)>
    MessageHandler;

  bool Initialize();
  void Finalize();

  inline void SetMessageHandler(session::protocol::GatewayMessageType::type type,
      const MessageHandler &handler) {
    this->handler_[type] = handler;
  }
  inline void ResetMessageHandler(session::protocol::GatewayMessageType::type type) {
    this->handler_[type].clear();
  }

  void OnMessage(SessionTerminal *terminal, const char *message, size_t size);

 private:
  GatewayMessageHandler();
  ~GatewayMessageHandler();

  void OnMessageUserLogin(SessionTerminal *terminal, const char *message, size_t size);
  void OnMessageUserLogout(SessionTerminal *terminal, const char *message, size_t size);
  void OnMessageKickoutUser(SessionTerminal *terminal, const char *message, size_t size);

  MessageHandler handler_[session::protocol::GatewayMessageType::MESSAGE_MAX];
};

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__GATEWAY__MESSAGE__HANDLER__H

