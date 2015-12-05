//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-02-04 09:58:03.
// File name: session_terminal.h
//
// Description:
// Define class SessionTerminal.
//

#ifndef __SESSION__SERVER__SESSION__TERMINAL__H
#define __SESSION__SERVER__SESSION__TERMINAL__H

#include <string>

#include "core/inet_address.h"
#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/connection.h"
#include "session_server/server/game_server_index.h"
#include "global/logging.h"
#include "global/server_type_types.h"
#include "global/thrift_packet.h"
#include "session_server/protocol/session_game_protocol_types.h"
#include "session_server/protocol/session_gateway_protocol_types.h"
#include "session_server/protocol/session_protocol_types.h"

namespace session {

namespace server {

class SessionTerminal : public core::Noncopyable {
 public:
  SessionTerminal();
  ~SessionTerminal();

  bool Initialize(const coresh::ConnectionPtr &connection,
      global::ThriftPacket *packet);
  void Finalize();

  template <typename T>
  void SendMessage(const T &message, protocol::MessageType::type type);

  void SendMessage(const protocol::Message &message);
  void SendMessage(const char *message, size_t size);

  // Send message to game server.
  template <typename T> 
  void SendGameMessage(const T &message, session::protocol::GameMessageType::type type);

  // Send message to gateway server.
  template <typename T>
  void SendGatewayMessage(const T &message,
      session::protocol::GatewayMessageType::type type);

  inline void SetSignature(const global::protocol::ServerSignature &signature) {
    this->signature_ = signature;
  }
  inline const global::protocol::ServerSignature &GetSignature() const {
    return this->signature_;
  }

  inline global::ThriftPacket *GetThrift() {
    return this->thrift_;
  }

 private:
  void OnDisconnect();
  void OnMessage(const char *message, size_t size);

  void OnMessageLogin(const char *message, size_t size);
  void OnMessageLogout(const char *message, size_t size);

  void OnMessageGame(const char *message, size_t size);
  void OnMessageGateway(const char *message, size_t size);

  void OnMessageForward(const char *message, size_t size);

  coresh::ConnectionPtr connection_;

  global::protocol::ServerSignature signature_;
  global::ThriftPacket *thrift_;
};

template <typename T>
void SessionTerminal::SendMessage(const T &message,
    protocol::MessageType::type type) {
  if(this->thrift_ == NULL) {
    global::LogError("%s:%d (%s) thrift is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  size_t size = 0;
  const char *message1 = this->thrift_->Serialize(message, size);
  if(message1) {
    protocol::Message message2;
    message2.__set_type_(type);
    message2.message_.assign(message1, size);
    this->SendMessage(message2);
  } else {
    global::LogError("%s:%d (%s) Serialize message[%d] failed.",
        __FILE__, __LINE__, __FUNCTION__, type);
  }
};

template <typename T>
void SessionTerminal::SendGameMessage(const T &message,
    session::protocol::GameMessageType::type type) {
  if(this->GetSignature().type_ != global::protocol::ServerType::GAME_SERVER) {
    global::LogError("%s:%d (%s) Error server type[%d]",
        __FILE__, __LINE__, __FUNCTION__, this->GetSignature().type_);
    return ;
  }
  if(this->thrift_ == NULL) {
    global::LogError("%s:%d (%s) thrift is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  size_t size = 0;
  const char *message1 = this->thrift_->Serialize(message, size);
  if(message1 == NULL) {
    global::LogError("%s:%d (%s) Serialize message[%d] failed.",
        __FILE__, __LINE__, __FUNCTION__, type);
    return ;
  }
  protocol::MessageGameServer message2;
  message2.__set_type_(type);
  message2.message_.assign(message1, size);
  this->SendMessage(message2, protocol::MessageType::MESSAGE_GAME_SERVER);
}

template <typename T>
void SessionTerminal::SendGatewayMessage(const T &message,
    session::protocol::GatewayMessageType::type type) {
  if(this->GetSignature().type_ != global::protocol::ServerType::GATEWAY_SERVER) {
    global::LogError("%s:%d (%s) Error server type[%d]",
        __FILE__, __LINE__, __FUNCTION__, this->GetSignature().type_);
    return ;
  }
  if(this->thrift_ == NULL) {
    global::LogError("%s:%d (%s) thrift is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  size_t size = 0;
  const char *message1 = this->thrift_->Serialize(message, size);
  if(message1 == NULL) {
    global::LogError("%s:%d (%s) Serialize message[%d] failed.",
        __FILE__, __LINE__, __FUNCTION__, type);
    return ;
  }
  protocol::MessageGatewayServer message2;
  message2.__set_type_(type);
  message2.message_.assign(message1, size);
  this->SendMessage(message2, protocol::MessageType::MESSAGE_GATEWAY_SERVER);
}

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__SESSION__TERMINAL__H

