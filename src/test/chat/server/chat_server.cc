//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2012-12-31 16:32:18.
// File name: chat_server.cc
//
// Description:
// Define class ChatServer.
//

#include "test/chat/server/chat_server.h"

#include "core/async_logging_singleton.h"
#include "test/chat/server/chat_session.h"

namespace test {

namespace chat {

namespace server {

ChatServer *ChatServerSingleton::server_;

bool ChatServer::Initialize(const std::string &log) {
  ChatServerSingleton::SetInstance(this);
  this->service_.SetConnectionCallback(boost::bind(&ChatServer::OnConnection, this, _1));
  return this->service_.Initialize(log);
}

bool ChatServer::Login(const std::string &name, ChatSession *session) {
  ChatSessionSet::iterator iterator1 = this->connection_sessions_.find(session);
  if(iterator1 != this->connection_sessions_.end()) {
    ChatSessionMap::iterator iterator = this->sessions_.find(name);
    if(iterator == this->sessions_.end()) {
      this->connection_sessions_.erase(iterator1);
      this->sessions_.insert(std::make_pair(name, session));
      /*
      protocol::MessageLoginBroadcast message;
      message.name_ = name;
      size_t retsize = 0;
      const char *command = this->thrift_.Serialize(message, retsize);
      if(command) {
        char buffer[1024];
        memcpy(buffer, command, retsize);
        this->BroadcastMessage(protocol::MessageType::MESSAGE_LOGIN_BROADCAST, buffer, retsize);
      } else {
        AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
            "%s:%d (%s) Serialize message failed",
            __FILE__, __LINE__, __FUNCTION__);
      }
      */
      return true;
    }
  }
  return false;
}

bool ChatServer::Logout(const std::string &name) {
  ChatSessionMap::iterator iterator = this->sessions_.find(name);
  if(iterator != this->sessions_.end()) {
    this->sessions_.erase(iterator);
    protocol::MessageLogoutBroadcast message;
    message.name_ = name;
    size_t retsize = 0;
    const char *command = this->thrift_.Serialize(message, retsize);
    if(command) {
      char buffer[1024];
      memcpy(buffer, command, retsize);
      this->BroadcastMessage(protocol::MessageType::MESSAGE_LOGOUT_BROADCAST, buffer, retsize);
    } else {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Serialize message failed",
          __FILE__, __LINE__, __FUNCTION__);
    }
    return true;
  }
  return false;
}

void ChatServer::BroadcastMessage(protocol::MessageType::type type,
    const char *message, size_t size) {
  ChatSessionMap::iterator iterator = this->sessions_.begin();
  for(; iterator != this->sessions_.end(); ++iterator) {
    iterator->second->SendMessage(type, message, size);
  }
}

void ChatServer::OnConnection(const coresh::ConnectionPtr &connection) {
  assert(connection);
  ChatSession *session = new (std::nothrow) ChatSession();
  if(session) {
    if(session->Initialize(connection) == true) {
      this->connection_sessions_.insert(session);
    } else {
      delete session;
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Initialize chat session failed.",
          __FILE__, __LINE__, __FUNCTION__);
    }
  }
}

void ChatServer::OnDisconnect(ChatSession *session) {
  assert(session);
  this->Logout(session->GetName());
  this->connection_sessions_.erase(session);
  session->Finalize();
  delete session;
}

}  // namespace server

}  // namespace chat

}  // namespace test

