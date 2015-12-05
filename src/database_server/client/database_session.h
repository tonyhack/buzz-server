//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-18 11:06:58.
// File name: database_session.h
//
// Description:
// Define class DatabaseSession.
//

#ifndef __DATABASE__CLIENT__DATABASE__SESSION__H
#define __DATABASE__CLIENT__DATABASE__SESSION__H

#include <boost/shared_ptr.hpp>

#include "core/async_logging_singleton.h"
#include "core/inet_address.h"
#include "core/base/noncopyable.h"
#include "coresh/client_side.h"
#include "global/server_type_types.h"
#include "global/thrift_packet.h"
#include "database_server/protocol/database_protocol_types.h"

namespace database {

namespace client {

class DatabaseSession : public core::Noncopyable,
  public boost::enable_shared_from_this<DatabaseSession> {
  enum StatusLogin { STATUS_NOT_LOGIN = 0, STATUS_LOGINED, STATUS_STOP, STATUS_FINAL, STATUS_MAX, };

 public:
  DatabaseSession();
  ~DatabaseSession();

  bool Initialize(const core::InetAddress &server_address,
      const std::string &name, core::EventLoop *network_loop, core::EventLoop *application_loop,
      const global::protocol::ServerSignature &signature, global::ThriftPacket *packet);
  void Finalize();

  void Start();
  void Stop();

  template <typename T>
  void SendMessage(const T &message, protocol::MessageType::type type);

  void SendMessage(const protocol::Message &message);
  void SendMessage(const char *message, size_t size);

  inline void SetServerSignature(const global::protocol::ServerSignature &signature) {
    this->signature_ = signature;
  }
  inline const global::protocol::ServerSignature &GetServerSignature() const {
    return this->signature_;
  }

  inline void SetStatus(StatusLogin status) {
    this->status_ = status;
  }
  inline StatusLogin GetStatus() const {
    return this->status_;
  }
  inline bool CheckLoginStatues() const {
    return this->GetStatus() == STATUS_LOGINED;
  }

 private:
  void OnConnection(const coresh::ConnectionPtr &connection);
  void OnMessage(const char *message, size_t size);
  void OnDisconnect();

  // Message handler.
  void OnMessageLoginResponse(const char *message, size_t size);
  void OnMessageLogoutResponse(const char *message, size_t size);
  void OnMessageStorageResponse(const char *message, size_t size);

  coresh::ConnectionPtr connection_;
  coresh::ClientSide client_;

  global::ThriftPacket *packet_;

  global::protocol::ServerSignature signature_;
  StatusLogin status_;
};

typedef boost::shared_ptr<DatabaseSession> DatabaseSessionPtr;

template <typename T>
void DatabaseSession::SendMessage(const T &message, protocol::MessageType::type type) {
  assert(this->packet_);
  size_t size = 0;
  const char *message1 = this->packet_->Serialize(message, size);
  if(message1) {
    protocol::Message message2;
    message2.__set_type_(type);
    message2.message_.assign(message1, size);
    this->SendMessage(message2);
  } else {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Serialize message[%d] failed.",
        __FILE__, __LINE__, __FUNCTION__, type);
  }
}

}  // namespace client

}  // namespace database

#endif  // __DATABASE__CLIENT__DATABASE__SESSION__H

