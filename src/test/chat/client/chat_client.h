//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-04 09:50:10.
// File name: chat_client.h
//
// Description:
// Define class ChatClient.
//

#ifndef __TEST__CHAT__CLIENT__CHAT__CLIENT__H
#define __TEST__CHAT__CLIENT__CHAT__CLIENT__H

#include "core/async_logging_singleton.h"
#include "core/inet_address.h"
#include "core/base/noncopyable.h"
#include "coresh/client_side.h"
#include "global/thrift_packet.h"
#include "test/chat/protocol/protocol_types.h"

namespace test {

namespace chat {

namespace client {

class ChatClient : public core::Noncopyable {
 public:
  ChatClient(const core::InetAddress &server_address, const std::string &name)
    : name_(name), client_(server_address, name), thread_(&loop_) {}
  ~ChatClient() {}

  void SetName(const std::string &name) {
    this->name_ = name;
  }
  const std::string &GetName() const {
    return this->name_;
  }

  core::EventLoop *GetEventLoop() {
    return &this->loop_;
  }

  bool Initialize() {
    if(this->loop_.Initialize(true) == false) {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Initialize event loop failed", __FILE__, __LINE__, __FUNCTION__);
      return false;
    }
    if(this->client_.Initialize(&this->loop_, &this->loop_,
          boost::bind(&ChatClient::OnConnection, this, _1)) == false) {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Initialize client_ failed", __FILE__, __LINE__, __FUNCTION__);
      return false;
    }
    return true;
  }

  void Start() {
    this->thread_.StartLoop();
    this->client_.Start();
  }

  void Stop() {
    this->client_.Stop();
    this->thread_.StopLoop();
  }

  void SendMessage(protocol::MessageType::type type,
      const char *message, size_t size) {
    protocol::Message msg;
    msg.__set_type_(type);
    msg.message_.assign(message, size);
    msg.__isset.message_ = true;
    size_t retsize = 0;
    const char *command = this->thrift_.Serialize(msg, retsize);
    if(command) {
      this->SendMessage(command, retsize);
    } else {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Serialize message failed.", __FILE__, __LINE__, __FUNCTION__);
    }
  }

  void SendMessage(const char *message, size_t size) {
    this->connection_->SendMessage(message, size);
  }

  void Input(const std::string &input) {
    std::string::size_type pos = input.find("@");
    if(pos != std::string::npos) {
      std::string command = input.substr(0, pos);
      protocol::MessageChat message;
      if(command == "broadcast") {
        message.__set_type_(protocol::ChatType::CHAT_BROADCAST);
        message.__set_from_(this->name_);
        message.__set_contect_(input.substr(pos + 1, std::string::npos));
      } else if(command == "private") {
        std::string input2 = input.substr(pos + 1, std::string::npos);
        std::string::size_type pos = input2.find("@");
        message.__set_type_(protocol::ChatType::CHAT_PRIVATE);
        message.__set_from_(this->name_);
        message.__set_to_(input2.substr(0, pos));
        message.__set_contect_(input2.substr(pos + 1, std::string::npos));
      } else {
        std::cout << "[system] Error input!" << std::endl;
        return ;
      }
      size_t retsize = 0;
      const char *msg = this->thrift_.Serialize(message, retsize);
      if(msg) {
        this->SendMessage(protocol::MessageType::MESSAGE_CHAT, msg, retsize);
      } else {
        AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
            "%s:%d (%s) Serialize message failed", __FILE__, __LINE__, __FUNCTION__);
      }
    } else {
      std::cout << "[system] Error input!" << std::endl;
    }
  }

 private:
  void OnConnection(const coresh::ConnectionPtr &connection) {
    this->connection_ = connection;
    this->connection_->SetDisconnectCallback(boost::bind(&ChatClient::OnDisconnect, this));
    this->connection_->SetMessageCallback(boost::bind(&ChatClient::OnMessage, this, _1, _2));
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_INFO,
        "Connect successful.");
    protocol::MessageLoginRequest message;
    message.__set_name_(this->name_);
    size_t retsize = 0;
    const char *command = this->thrift_.Serialize(message, retsize);
    if(command) {
      this->SendMessage(protocol::MessageType::MESSAGE_LOGIN_REQUEST, command, retsize);
    } else {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Serialize message failed", __FILE__, __LINE__, __FUNCTION__);
    }
  }
  void OnDisconnect() {
    this->connection_.reset();
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_INFO,
        "Disconnected server.");
  }
  void OnMessage(const char *message, size_t size) {
    protocol::Message msg;
    if(this->thrift_.Deserialize(msg, message, size)) {
      switch(msg.type_) {
        case protocol::MessageType::MESSAGE_LOGIN_RESPONSE:
          this->OnLoginResponse(msg.message_.data(), msg.message_.size());
          break;
        case protocol::MessageType::MESSAGE_LOGIN_BROADCAST:
          this->OnLoginBroadcastMessage(msg.message_.data(), msg.message_.size());
          break;
        case protocol::MessageType::MESSAGE_LOGOUT_BROADCAST:
          this->OnLogoutBroadcastMessage(msg.message_.data(), msg.message_.size());
          break;
        case protocol::MessageType::MESSAGE_CHAT:
          this->OnChatMessage(msg.message_.data(), msg.message_.size());
          break;
        default:
          AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
              "%s:%d (%s) Error message type[%d]",
              __FILE__, __LINE__, __FUNCTION__, (int)msg.type_);
          break;
      }
    } else {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Deserialize message failed",
          __FILE__, __LINE__, __FUNCTION__);
    }
  }

  void OnLoginResponse(const char *message, size_t size) {
    protocol::MessageLoginResponse msg;
    if(this->thrift_.Deserialize(msg, message, size)) {
      if(msg.success_) {
        std::cout << "[system] you login the chat." << std::endl;
      } else {
        std::cout << "[system] login failed." << std::endl;
      }
    } else {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Deserialize message failed",
          __FILE__, __LINE__, __FUNCTION__);
    }
  }

  void OnLoginBroadcastMessage(const char *message, size_t size) {
    protocol::MessageLoginBroadcast msg;
    if(this->thrift_.Deserialize(msg, message, size)) {
      std::cout << "[system] user " << msg.name_ << " login chat." << std::endl;
    } else {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Deserialize message[%llu] failed",
          __FILE__, __LINE__, __FUNCTION__, size);
    }
  }

  void OnLogoutBroadcastMessage(const char *message, size_t size) {
    protocol::MessageLogoutBroadcast msg;
    if(this->thrift_.Deserialize(msg, message, size)) {
      std::cout << "[system] user " << msg.name_ << " logout chat." << std::endl;
    } else {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Deserialize message failed",
          __FILE__, __LINE__, __FUNCTION__);
    }
  }

  void OnChatMessage(const char *message, size_t size) {
    protocol::MessageChat msg;
    if(this->thrift_.Deserialize(msg, message, size)) {
      if(msg.type_ == protocol::ChatType::CHAT_BROADCAST) {
        std::cout << msg.from_ << ": " << msg.contect_ << std::endl;
      } else if(msg.type_ == protocol::ChatType::CHAT_PRIVATE) {
        std::cout << msg.from_ << " to you: " << msg.contect_ << std::endl;
      } else {
        AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
            "%s:%d (%s) Error chat type[%d]",
            __FILE__, __LINE__, __FUNCTION__, (int)msg.type_);
      }
    } else {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Deserialize message failed",
          __FILE__, __LINE__, __FUNCTION__);
    }
  }

  std::string name_;

  coresh::ConnectionPtr connection_;

  coresh::ClientSide client_;

  core::EventLoop loop_;
  core::LoopThread thread_;

  global::ThriftPacket thrift_;
};

}  // namespace client

}  // namespace chat

}  // namespace test

#endif // __TEST__CHAT__CLIENT__CHAT__CLIENT__H

