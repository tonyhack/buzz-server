#ifndef __SESSION__SERVER__ADMIN__ADMIN__SESSION__H
#define __SESSION__SERVER__ADMIN__ADMIN__SESSION__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/connection.h"
#include "global/global_packet.h"
#include "global/logging.h"
#include "session_server/protocol/session_admin_protocol_types.h"
#include "session_server/server/session_server.h"

namespace session {

namespace server {

namespace admin {

class AdminSession : public core::Noncopyable {
 public:
  AdminSession();
  ~AdminSession();

  bool Initialize(uint64_t session_id, const coresh::ConnectionPtr &connection);
  void Finalize();
  void Close();

  uint64_t GetID() const { return this->session_id_; }

  template <typename T>
  void SendMessage(const T &message, session::protocol::AdminMessageType::type type);

 private:
  void OnMessage(const char *message, size_t size);

  uint64_t session_id_;
  coresh::ConnectionPtr connection_;
};

template <typename T>
void AdminSession::SendMessage(const T &message,
    session::protocol::AdminMessageType::type type) {
  global::ThriftPacket *thrift = SessionServerSingleton::GetInstance().GetThrift();
  if (NULL == thrift) {
    LOG_ERROR("thrift is null.");
    return;
  }
  size_t size = 0;
  const char *message1 = thrift->Serialize(message, size);
  if (NULL == message1) {
    LOG_ERROR("Serialize message[%d] failed.", type);
    return;
  }

  protocol::MessageAdmin message2;
  message2.__set_type_(type);
  message2.message_.assign(message1, size);

  const char *message_out = thrift->Serialize(message2, size);
  if (NULL == message_out) {
    LOG_ERROR("Serialize message[%d] failed.", type);
    return;
  }

  this->connection_->SendMessage(message_out, size);
}
  
}  // namespace admin

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__ADMIN__ADMIN__SESSION__H

