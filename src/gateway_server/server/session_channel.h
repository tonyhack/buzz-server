//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-02-27 12:27:38.
// File name: session_channel.h
//
// Description:
// Define class SessionChannel.
//

#ifndef __GATEWAY__SERVER__SESSION__CHANNEL__H
#define __GATEWAY__SERVER__SESSION__CHANNEL__H

#include "core/base/noncopyable.h"
#include "gateway_server/server/gateway_server.h"
#include "global/thrift_packet.h"
#include "session_server/client/session_client.h"
#include "session_server/protocol/session_gateway_protocol_types.h"
#include "session_server/protocol/session_protocol_types.h"

namespace gateway {

namespace server {

class SessionChannel : public core::Noncopyable {
 public:
  static bool CheckSessionConnection() {
    session::client::SessionClientPtr session =
      GatewayServerSingleton::GetInstance().GetSessionClient();
    return session.get() != NULL;
  }

  template <typename T>
  static void SendMessage(const T &message,
      session::protocol::GatewayMessageType::type type) {
    global::ThriftPacket *thrift = GatewayServerSingleton::GetInstance().GetThrift();
    if(thrift == NULL) {
      global::LogError("%s:%d (%s) thrift is null.",
          __FILE__, __LINE__, __FUNCTION__);
      return ;
    }
    session::client::SessionClientPtr session =
      GatewayServerSingleton::GetInstance().GetSessionClient();
    if(session.get() == NULL) {
      global::LogError("%s:%d (%s) session is null.",
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
    session::protocol::MessageGatewayServer message2;
    message2.__set_type_(type);
    message2.message_.assign(message1, size);
    SessionChannel::SendMessage(message2);
  }

  static void SendMessage(const session::protocol::MessageGatewayServer &message) {
    session::client::SessionClientPtr session =
      GatewayServerSingleton::GetInstance().GetSessionClient();
    if(session.get() == NULL) {
      global::LogError("%s:%d (%s) Session client is null",
          __FILE__, __LINE__, __FUNCTION__);
    }
    session->SendMessage(message, session::protocol::MessageType::MESSAGE_GATEWAY_SERVER);
  }
};

}  // namespace server

}  // namespace game

#endif  // __GATEWAY__SERVER__SESSION__CHANNEL__H

