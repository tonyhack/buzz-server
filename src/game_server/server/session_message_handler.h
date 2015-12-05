//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-02-25 17:57:38.
// File name: session_message_handler.h
//
// Description:
// Define class SessionMessageHandler.
//

#ifndef __GAME__SERVER__SESSION__MESSAGE__HANDLER__H
#define __GAME__SERVER__SESSION__MESSAGE__HANDLER__H


#include <boost/function.hpp>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "global/thrift_packet.h"
#include "session_server/protocol/session_game_protocol_types.h"

namespace game {

namespace server {

class SessionMessageHandler : public core::Noncopyable {
 public:
  typedef boost::function<void (const char *, size_t)> MessageHandler;

  SessionMessageHandler();
  ~SessionMessageHandler();

  bool Initialize(global::ThriftPacket *thrift);
  void Finalize();

  void OnMessageGameServer(const char *message, size_t size);

  inline void SetMessageHandler(session::protocol::GameMessageType::type type,
      const MessageHandler &handler) {
    this->handler_[type] = handler;
  }
  inline void ResetMessageHandler(session::protocol::GameMessageType::type type) {
    this->handler_[type].clear();
  }

 private:
  void OnMessageUserLogin(const char *message, size_t size);
  void OnMessageUserLogout(const char *message, size_t size);

  void ResponseSession(core::uint64 actor, bool success);

  global::ThriftPacket *thrift_;
  MessageHandler handler_[session::protocol::GameMessageType::MESSAGE_MAX];
};

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SESSION__MESSAGE__HANDLER__H

