//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-08 14:39:48.
// File name: schedule_terminal.h
//
// Description:
// Define class ScheduleTerminal.
//

#ifndef __SCHEDULE__SERVER__SCHEDULE__TERMINAL__H
#define __SCHEDULE__SERVER__SCHEDULE__TERMINAL__H

#include <string>

#include "core/async_logging_singleton.h"
#include "core/inet_address.h"
#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/connection.h"
#include "global/server_type_types.h"
#include "global/thrift_packet.h"
#include "schedule_server/protocol/schedule_protocol_types.h"

namespace schedule {

namespace server {

class ScheduleTerminal : public core::Noncopyable {
 public:
  ScheduleTerminal();
  ~ScheduleTerminal();

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

  void OnMessageUploadGatewayUserNumber(const char *message, size_t size);

  coresh::ConnectionPtr connection_;

  global::protocol::ServerSignature signature_;

  global::ThriftPacket *packet_;
};

template <typename T>
void ScheduleTerminal::SendMessage(const T &message,
    protocol::MessageType::type type) {
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
};

}  // namespace server

}  // namespace schedule

#endif  // __SCHEDULE__SERVER__SCHEDULE__TERMINAL__H

