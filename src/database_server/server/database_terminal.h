//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-15 11:41:18.
// File name: database_terminal.h
//
// Description:
// Define class DatabaseTerminal.
//

#ifndef __DATABASE__SERVER__DATABASE__TERMINAL__H
#define __DATABASE__SERVER__DATABASE__TERMINAL__H

#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include "core/async_logging_singleton.h"
#include "core/tcp_connection.h"
#include "core/base/mutex.h"
#include "core/base/noncopyable.h"
#include "coresh/packetizer.h"
#include "database_server/protocol/database_protocol_types.h"
#include "global/server_type_types.h"
#include "global/thrift_packet.h"

namespace database {

namespace server {

class DatabaseTerminal : public core::Noncopyable,
  public boost::enable_shared_from_this<DatabaseTerminal> {
  typedef boost::weak_ptr<core::TCPConnection> TCPConnectionWeakPtr;

 public:
  DatabaseTerminal();
  ~DatabaseTerminal();

  bool Initialize(const core::TCPConnectionPtr &connection);
  void Finalize();

  inline void SetSignature(const global::protocol::ServerSignature &signature) {
    this->signature_ = signature;
  }
  inline const global::protocol::ServerSignature &GetSignature() const {
    return this->signature_;
  }

  inline bool GetLoginStatus() const {
    return this->login_status_;
  }
  inline void SetLoginStatus(bool status) {
    this->login_status_ = status;
  }

  void OnMessage(const char *message, size_t size);
  void OnDisconnected();

  // Thread-safe send.
  void SendMessage(const char *message, size_t size, coresh::Packetizer *packetizer);

 private:
  // Non-thread-safe, Only OnMessage/OnDisconnected/Initialize can invoke.
  template <typename T>
  void SendMessage(const T &message, protocol::MessageType::type type);
  void SendMessage(const protocol::Message &message);

  void OnMessageLogin(const char *message, size_t size);
  void OnMessageLogout(const char *message, size_t size);
  void OnMessageStorage(const char *message, size_t size);

  void ErrorResponse(protocol::ResponseType::type type,
      const protocol::MessageStorageRequest &request);

  global::protocol::ServerSignature signature_;

  TCPConnectionWeakPtr connection_;

  global::ThriftPacket packet_;
  coresh::Packetizer *packetizer_;

  core::Mutex mutex_;
  bool login_status_;
};

typedef boost::shared_ptr<DatabaseTerminal> DatabaseTerminalPtr;

template <typename T>
void DatabaseTerminal::SendMessage(const T &message,
    protocol::MessageType::type type) {
  size_t size = 0;
  const char *message1 = this->packet_.Serialize(message, size);
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

}  // namespace server

}  // namespace database

#endif  // __DATABASE__SERVER__DATABASE__TERMINAL__H

