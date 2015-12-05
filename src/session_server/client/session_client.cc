//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-02-06 18:19:19.
// File name: session_client.cc
//
// Description:
// Define class SessionClient.
//

#include "session_server/client/session_client.h"

#include <boost/bind.hpp>

namespace session {

namespace client {

SessionClient::SessionClient() : status_(STATUS_NOT_LOGIN), thrift_(NULL) {}
SessionClient::~SessionClient() {}

bool SessionClient::Initialize(const core::InetAddress &server_address, const std::string &name,
    core::EventLoop *network_loop, core::EventLoop *application_loop,
    const global::protocol::ServerSignature &signature, global::ThriftPacket *thrift) {
  if(network_loop == NULL) {
    global::LogError("%s:%d (%s) Argument network loop is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(application_loop == NULL) {
    global::LogError("%s:%d (%s) Argument application loop is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(thrift == NULL) {
    global::LogError("%s:%d (%s) Argument thrift is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  this->thrift_ = thrift;
  if(this->client_.Initialize(server_address, name, network_loop, application_loop,
      boost::bind(&SessionClient::OnConnection, this, _1)) == false) {
    global::LogError("%s:%d (%s) Initialize client failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  this->SetServerSignature(signature);

  this->SetMessageHandler(protocol::MessageType::MESSAGE_LOGIN_RESPONSE,
      boost::bind(&SessionClient::OnMessageLoginResponse, this, _1, _2));
  this->SetMessageHandler(protocol::MessageType::MESSAGE_LOGOUT_RESPONSE,
      boost::bind(&SessionClient::OnMessageLogoutResponse, this, _1, _2));

  return true;
}

void SessionClient::Finalize() {
  this->ResetMessageHandler(protocol::MessageType::MESSAGE_LOGIN_RESPONSE);
  this->ResetMessageHandler(protocol::MessageType::MESSAGE_LOGOUT_RESPONSE);
  this->client_.Finalize();
  // Reset callbacks.
  if(this->connection_.get() != NULL) {
    this->connection_->ResetDisconnectCallback();
    this->connection_->ResetMessageCallback();
    this->connection_->Finalize();
  }
}

void SessionClient::Start() {
  this->SetStatus(STATUS_NOT_LOGIN);
  this->client_.Start();
}

void SessionClient::Stop() {
  this->client_.Stop();
  this->SetStatus(STATUS_STOP);
}

void SessionClient::SendMessage(const protocol::Message &message) {
  if(this->thrift_ == NULL) {
    global::LogError("%s:%d (%s) Thrift packet is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  size_t size = 0;
  const char *message1 = this->thrift_->Serialize(message, size);
  if(message1 == NULL) {
    global::LogError("%s:%d (%s) Deserialize message failed",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  } 
  this->SendMessage(message1, size);
}

void SessionClient::SendMessage(const char *message, size_t size) {
  if(this->connection_.get() == NULL) {
    global::LogError("%s:%d (%s) Connection is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  this->connection_->SendMessage(message, size);
}

void SessionClient::OnConnection(const coresh::ConnectionPtr &connection) {
  if(connection.get() == NULL) {
    global::LogError("%s:%d (%s) Argument connection is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  this->connection_ = connection;
  this->connection_->SetDisconnectCallback(
      boost::bind(&SessionClient::OnDisconnect, this));
  this->connection_->SetMessageCallback(
      boost::bind(&SessionClient::OnMessage, this, _1, _2));

  global::LogInfo("Connection session server[host=%s] completely.",
      this->client_.GetServerAddress().ToString().c_str());

  // Send login request message.
  protocol::MessageLoginRequest message;
  message.signature_ = this->GetServerSignature();
  this->SendMessage(message, protocol::MessageType::MESSAGE_LOGIN_REQUEST);
}

void SessionClient::OnMessage(const char *message, size_t size) {
  if(this->thrift_ == NULL) {
    global::LogError("%s:%d (%s) Thrift packet is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  protocol::Message message2;
  if(this->thrift_->Deserialize(message2, message, size) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  if(message2.type_ < protocol::MessageType::MESSAGE_MIN ||
      message2.type_ >= protocol::MessageType::MESSAGE_MAX ||
      message_handlers_[message2.type_].empty() == true) {
    global::LogError("%s:%d (%s) Error message type[%d]",
        __FILE__, __LINE__, __FUNCTION__, message2.type_);
    return ;
  }
  this->message_handlers_[message2.type_](message2.message_.data(),
      message2.message_.size());
}

void SessionClient::OnDisconnect() {
  if(this->connection_.get() == NULL) {
    global::LogError("%s:%d (%s) Connection is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  this->connection_->ResetDisconnectCallback();
  this->connection_->ResetMessageCallback();
  this->connection_.reset();
  global::LogInfo("Disconnected session server[host=%s:%d], reconnect it.",
      this->GetServerSignature().host_.c_str(), this->GetServerSignature().port_);
  this->client_.Start();
}

void SessionClient::OnMessageLoginResponse(const char *message, size_t size) {
  if(this->thrift_ == NULL) {
    global::LogError("%s:%d (%s) Thrift packet is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  protocol::MessageLoginResponse message2;
  if(this->thrift_->Deserialize(message2, message, size) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  if(message2.logined_ == false) {
    global::LogError("%s:%d (%s) Login session server failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  global::LogInfo("Login session server[host=%s] successful",
      this->client_.GetServerAddress().ToString().c_str());
  this->SetStatus(STATUS_LOGINED);

  // Login success callback.
  if(this->login_callback_.empty() == false) {
    this->login_callback_();
  }
}

void SessionClient::OnMessageLogoutResponse(const char *message, size_t size) {
  if(this->thrift_ == NULL) {
    global::LogError("%s:%d (%s) Thrift packet is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  protocol::MessageLogoutResponse message2;
  if(this->thrift_->Deserialize(message2, message, size) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  if(message2.logouted_ == false) {
    global::LogError("%s:%d (%s) Logout session server failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  global::LogInfo("Logout session server[host=%s] successful",
      this->client_.GetServerAddress().ToString().c_str());
  this->SetStatus(STATUS_NOT_LOGIN);
}

}  // namespace client

}  // namespace session

