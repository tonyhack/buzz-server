//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-02-06 17:35:42.
// File name: session_client.h
//
// Description:
// Define class SessionClient.
//

#ifndef __SESSION__CLIENT__SESSION__CLIENT__H
#define __SESSION__CLIENT__SESSION__CLIENT__H

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

#include "core/inet_address.h"
#include "core/base/noncopyable.h"
#include "coresh/client_side.h"
#include "global/logging.h"
#include "global/server_type_types.h"
#include "global/thrift_packet.h"
#include "session_server/protocol/session_protocol_types.h"

namespace session {

namespace client {

class SessionClient : public core::Noncopyable {
  enum Status { STATUS_NOT_LOGIN = 0, STATUS_LOGINED, STATUS_STOP, STATUS_FINAL, STATUS_MAX, };
  typedef boost::function<void (const char *, size_t)> MessageHandler;

  typedef boost::function<void()> LoginCallback;

 public:
  SessionClient();
  ~SessionClient();

  bool Initialize(const core::InetAddress &server_address, const std::string &name,
      core::EventLoop *network_loop, core::EventLoop *application_loop,
      const global::protocol::ServerSignature &signature, global::ThriftPacket *thrift);
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

  // Message handler.
  inline void SetMessageHandler(protocol::MessageType::type type,
      const MessageHandler &callback) {
    this->message_handlers_[type] = callback;
  }
  inline void ResetMessageHandler(protocol::MessageType::type type) {
    this->message_handlers_[type].clear();
  }

  inline void SetLoginCallback(const LoginCallback &callback) {
    this->login_callback_ = callback;
  }

  inline void ResetLoginCallback() {
    this->login_callback_.clear();
  }

  inline bool CheckLoginStatus() const {
    return this->GetStatus() == STATUS_LOGINED;
  }

 private:
  void OnConnection(const coresh::ConnectionPtr &connection);
  void OnMessage(const char *message, size_t size);
  void OnDisconnect();

  // Message handler.
  void OnMessageLoginResponse(const char *message, size_t size);
  void OnMessageLogoutResponse(const char *message, size_t size);

  inline Status GetStatus() const {
    return this->status_;
  }
  inline void SetStatus(Status status) {
    this->status_ = status;
  }

  Status status_;

  coresh::ConnectionPtr connection_;
  coresh::ClientSide client_;

  global::ThriftPacket *thrift_;

  global::protocol::ServerSignature signature_;

  MessageHandler message_handlers_[protocol::MessageType::MESSAGE_MAX];

  LoginCallback login_callback_;
};

typedef boost::shared_ptr<SessionClient> SessionClientPtr;

template <typename T>
void SessionClient::SendMessage(const T &message, protocol::MessageType::type type) {
  if(this->thrift_ == NULL) {
    global::LogError("%s:%d (%s) Thrift is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  size_t size = 0;
  const char *message1 = this->thrift_->Serialize(message, size);
  if(message1 == NULL) {
    global::LogError("%s:%d (%s) Serialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  protocol::Message message2;
  message2.__set_type_(type);
  message2.message_.assign(message1, size);
  this->SendMessage(message2);
}

}  // namespace client

}  // namespace session

#endif  // __SESSION__CLIENT__SESSION__CLIENT__H

