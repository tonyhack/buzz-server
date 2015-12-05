//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-10 17:09:08.
// File name: schedule_client.h
//
// Description:
// Define class ScheduleClient.
//

#ifndef __SCHEDULE__CLIENT__SCHEDULE__CLIENT__h
#define __SCHEDULE__CLIENT__SCHEDULE__CLIENT__h

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

#include "core/async_logging_singleton.h"
#include "core/inet_address.h"
#include "core/base/noncopyable.h"
#include "coresh/client_side.h"
#include "global/thrift_packet.h"
#include "global/server_type_types.h"
#include "schedule_server/protocol/schedule_protocol_types.h"

namespace schedule {

namespace client {

class ScheduleClient : public core::Noncopyable {
  typedef boost::function<void (const global::protocol::ServerSignature &)> JoinScheduleCallback;
  typedef boost::function<void (const char *, size_t)> MessageHandler;

 public:
  ScheduleClient();
  ~ScheduleClient();

  bool Initialize(const core::InetAddress &server_address, const std::string &name,
      core::EventLoop *network_loop, core::EventLoop *application_loop,
      const global::protocol::ServerSignature &signature, global::ThriftPacket *packet);
  void Finalize();

  void Start();
  void Stop();

  template <typename T>
  void SendMessage(const T &message, protocol::MessageType::type type);

  void SendMessage(const protocol::Message &message);
  void SendMessage(const char *message, size_t size);

  // When other terminal join schedule, this will be invoke.
  inline void SetJoinScheduleCallback(const JoinScheduleCallback &callback) {
    this->join_callback_ = callback;
  }
  inline void ResetJoinSchedulecallback() {
    this->join_callback_.clear();
  }

  // When other terminal disjoin schedule, this will be invoke.
  inline void SetDisjoinScheduleCallback(const JoinScheduleCallback &callback) {
    this->disjoin_callback_ = callback;
  }
  inline void ResetDisjoinScheduleCallback() {
    this->disjoin_callback_.clear();
  }

  inline void SetServerSignature(const global::protocol::ServerSignature &signature) {
    this->signature_ = signature;
  }
  inline const global::protocol::ServerSignature &GetServerSignature() const {
    return this->signature_;
  }

  // Message handler.
  inline void SetMessageHandler(protocol::MessageType::type type,
      const MessageHandler &callback) {
    this->message_handlers_[type] = callback;
  }
  inline void ResetMessageHandler(protocol::MessageType::type type) {
    this->message_handlers_[type].clear();
  }

 private:
  void OnConnection(const coresh::ConnectionPtr &connection);
  void OnMessage(const char *message, size_t size);
  void OnDisconnect();

  // Message handler.
  void OnMessageLoginResponse(const char *message, size_t size);
  void OnMessageLogoutResponse(const char *message, size_t size);
  void OnMessageLoginBroadcast(const char *message, size_t size);
  void OnMessageLogoutBroadcast(const char *message, size_t size);

  coresh::ConnectionPtr connection_;
  coresh::ClientSide client_;

  global::ThriftPacket *packet_;

  JoinScheduleCallback join_callback_;
  JoinScheduleCallback disjoin_callback_;

  global::protocol::ServerSignature signature_;

  MessageHandler message_handlers_[protocol::MessageType::MESSAGE_MAX];
};

typedef boost::shared_ptr<ScheduleClient> ScheduleClientPtr;

template <typename T>
void ScheduleClient::SendMessage(const T &message, protocol::MessageType::type type) {
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

}  // namespace schedule

#endif  // __SCHEDULE__CLIENT__SCHEDULE__CLIENT__h

