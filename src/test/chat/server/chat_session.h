//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2012-12-31 16:10:57.
// File name: chat_session.h
//
// Description:
// Define class ChatSession.
//

#ifndef __TEST__CHAT__SERVER__CHAT__SESSION__H
#define __TEST__CHAT__SERVER__CHAT__SESSION__H

#include <boost/bind.hpp>

#include "core/base/noncopyable.h"
#include "coresh/connection.h"
#include "test/chat/server/chat_server.h"
#include "test/chat/protocol/protocol_types.h"

namespace test {

namespace chat {

namespace server {

class ChatSession : public core::Noncopyable {
 public:
  ChatSession() : logined_(false) {}
  ~ChatSession() {}

  bool Initialize(const coresh::ConnectionPtr &connection) {
    assert(connection);
    this->connection_ = connection;
    this->connection_->SetDisconnectCallback(boost::bind(&ChatSession::OnDisconnect, this));
    this->connection_->SetMessageCallback(boost::bind(&ChatSession::OnMessage, this, _1, _2));
    return true;
  }

  void Finalize() {
    this->connection_->ResetDisconnectCallback();
    this->connection_->ResetMessageCallback();
    this->connection_.reset();
  }

  void SendMessage(protocol::MessageType::type type, const char *message, size_t size) {
    global::ThriftPacket *packet = ChatServerSingleton::GetInstance().GetThrift();
    if(packet) {
      protocol::Message msg;
      msg.__set_type_(type);
      msg.message_.assign(message, size);
      msg.__isset.message_ = true;
      size_t retsize = 0;
      const char *command = packet->Serialize(msg, retsize);
      if(command) {
        this->SendMessage(command, retsize);
      } else {
        AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
            "%s:%d (%s) Serialize message failed.", __FILE__, __LINE__, __FUNCTION__);
      }
    }
  }

  void SendMessage(const char *message, size_t size) {
    this->connection_->SendMessage(message, size);
  }

  inline const std::string &GetName() const { return this->name_; }

 private:
  void OnDisconnect() {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_INFO,
        "Session[%s] exit.", this->GetName().c_str());
    ChatServerSingleton::GetInstance().OnDisconnect(this);
  }

  void OnMessage(const char *message, size_t size) {
    global::ThriftPacket *packet = ChatServerSingleton::GetInstance().GetThrift();
    protocol::Message msg;
    if(packet->Deserialize(msg, message, size)) {
      switch(msg.type_) {
        case protocol::MessageType::MESSAGE_LOGIN_REQUEST:
          this->OnLoginMessage(msg.message_.data(), msg.message_.size());
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

  void OnLoginMessage(const char *message, size_t size) {
    global::ThriftPacket *packet = ChatServerSingleton::GetInstance().GetThrift();
    protocol::MessageLoginRequest msg;
    if(packet->Deserialize(msg, message, size)) {
      protocol::MessageLoginResponse command;
      if(ChatServerSingleton::GetInstance().Login(msg.name_, this)) {
        command.__set_success_(true);
        this->name_ = msg.name_;
      } else {
        command.__set_success_(false);
      }
      size_t retsize = 0;
      const char *response = packet->Serialize(command, retsize);
      if(response) {
        this->SendMessage(protocol::MessageType::MESSAGE_LOGIN_RESPONSE, response, retsize);
      } else {
        AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
            "%s:%d (%s) Serialize message failed",
            __FILE__, __LINE__, __FUNCTION__);
      }
    } else {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Deserialize message failed",
          __FILE__, __LINE__, __FUNCTION__);
    }
  }

  void OnChatMessage(const char *message, size_t size) {
    global::ThriftPacket *packet = ChatServerSingleton::GetInstance().GetThrift();
    protocol::MessageError error;
    protocol::MessageChat chat;
    if(packet->Deserialize(chat, message, size)) {
      if(chat.type_ == protocol::ChatType::CHAT_PRIVATE) {
        if(chat.__isset.to_) {
          ChatSession *session = ChatServerSingleton::GetInstance().GetSession(chat.to_);
          if(session) {
            session->SendMessage(protocol::MessageType::MESSAGE_CHAT, message, size);
            return ;
          } else {
            error.contect_ = "Can't find this session user.";
          }
        } else {
          error.contect_ = "Message error.";
        }
      } else if(chat.type_ == protocol::ChatType::CHAT_BROADCAST) {
        ChatServerSingleton::GetInstance().BroadcastMessage(protocol::MessageType::MESSAGE_CHAT,
            message, size);
        return ;
      } else {
        error.contect_ = "Error chat type.";
      }
    } else {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Deserialize message failed",
          __FILE__, __LINE__, __FUNCTION__);
      error.contect_ = "Message error.";
    }
    size_t retsize = 0;
    const char *command = packet->Serialize(error, retsize);
    if(command) {
      this->SendMessage(protocol::MessageType::MESSAGE_ERROR, command, retsize);
    } else {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Serialize message failed",
          __FILE__, __LINE__, __FUNCTION__);
    }
  }

  coresh::ConnectionPtr connection_;

  bool logined_;
  std::string name_;
};

}  // namespace server

}  // namespace chat

}  // namespace test

#endif  // __TEST__CHAT__SERVER__CHAT__SESSION__H

