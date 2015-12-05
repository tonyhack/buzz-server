//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-02-06 09:38:47.
// File name: game_terminal.h
//
// Description:
// Define class GameTerminal.
//

#ifndef __GAME__SERVER__GAME__TERMINAL__H
#define __GAME__SERVER__GAME__TERMINAL__H

#include <string>

#include "core/inet_address.h"
#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/connection.h"
#include "game_server/protocol/game_protocol_types.h"
#include "global/logging.h"
#include "global/server_type_types.h"
#include "global/thrift_packet.h"

namespace game {

namespace server {

class GameTerminal : public core::Noncopyable {
 public:
  GameTerminal();
  ~GameTerminal();

  bool Initialize(const coresh::ConnectionPtr &connection,
      global::ThriftPacket *packet);
  void Finalize();

  template <typename T>
  void SendMessage(const T &message, protocol::MessageType::type type);

  void SendMessage(const protocol::Message &message);
  void SendMessage(const char *message, size_t size);

  inline void SetSignature(const global::protocol::ServerSignature &signature) {
    this->signature_ = signature;
  }
  inline const global::protocol::ServerSignature &GetSignature() const {
    return this->signature_;
  }

 private:
  void OnDisconnect();
  void OnMessage(const char *message, size_t size);

  void OnMessageLogin(const char *message, size_t size);
  void OnMessageLogout(const char *message, size_t size);

  void OnMessageForward(const char *message, size_t size);

  void OnMessageActorLogout(const char *data, size_t size);

  coresh::ConnectionPtr connection_;

  global::protocol::ServerSignature signature_;
  global::ThriftPacket *thrift_;
};

template <typename T>
void GameTerminal::SendMessage(const T &message,
    protocol::MessageType::type type) {
  if(this->thrift_ == NULL) {
    global::LogError("%s:%d (%s) Thrift is null.",
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

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__GAME__TERMINAL__H

