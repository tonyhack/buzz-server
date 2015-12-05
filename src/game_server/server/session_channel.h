//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-02-26 10:24:48.
// File name: session_channel.h
//
// Description:
// Define class SessionChannel.
//

#ifndef __GAME__SERVER__SESSION__CHANNEL__H
#define __GAME__SERVER__SESSION__CHANNEL__H

#include "core/base/noncopyable.h"
#include "game_server/server/game_server.h"
#include "global/thrift_packet.h"
#include "session_server/client/session_client.h"
#include "session_server/protocol/session_game_protocol_types.h"
#include "session_server/protocol/session_protocol_types.h"

namespace game {

namespace server {

class SessionChannel : public core::Noncopyable {
 public:
  template <typename T>
  static void SendMessage(const T &message,
      session::protocol::GameMessageType::type type) {
    global::ThriftPacket *thrift = GameServerSingleton::GetInstance().GetThrift();
    if(thrift == NULL) {
      global::LogError("%s:%d (%s) thrift is null.",
          __FILE__, __LINE__, __FUNCTION__);
      return ;
    }
    size_t size = 0;
    const char *message1 = thrift->Serialize(message, size);
    if(message1 == NULL) {
      global::LogError("%s:%d (%s) Serialize message[%d] failed.",
          __FILE__, __LINE__, __FUNCTION__, type);
      return ;
    }
    session::protocol::MessageGameServer message2;
    message2.__set_type_(type);
    message2.message_.assign(message1, size);
    SessionChannel::SendMessage(message2);
  }

  static void SendMessage(const session::protocol::MessageGameServer &message) {
    session::client::SessionClientPtr session =
      GameServerSingleton::GetInstance().GetSessionClient();
    if(session.get() == NULL) {
      global::LogError("%s:%d (%s) Session client is null",
          __FILE__, __LINE__, __FUNCTION__);
      return ;
    }
    session->SendMessage(message, session::protocol::MessageType::MESSAGE_GAME_SERVER);
  }

  inline static bool CheckSessionLogined() {
    return SessionChannel::logined_;
  }

  inline static void SetSessionLogined(bool logined) {
    SessionChannel::logined_ = logined;
  }

 private:
  static bool logined_;
};

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SESSION__CHANNEL__H

