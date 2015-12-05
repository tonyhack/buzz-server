//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2012-12-31 15:08:19.
// File name: chat_server.h
//
// Description:
// Define class ChatServer.
//

#ifndef __TEST__CHAT__SERVER__CHAT__SERVER__H
#define __TEST__CHAT__SERVER__CHAT__SERVER__H

#include <map>
#include <set>
#include <string>

#include <boost/bind.hpp>

#include "core/base/noncopyable.h"
#include "coresh/application_service.h"
#include "coresh/connection.h"
#include "global/thrift_packet.h"
#include "test/chat/protocol/protocol_types.h"

namespace test {

namespace chat {

namespace server {

class ChatSession;

class ChatServer : public core::Noncopyable {
  typedef std::map<std::string, ChatSession *> ChatSessionMap;
  typedef std::set<ChatSession *> ChatSessionSet;
 public:
  ChatServer(const core::InetAddress &address, const std::string &name)
    : service_(address, name) {}
  ~ChatServer() {}

  bool Initialize(const std::string &log);

  void Finalize() {
    this->service_.Finalize();
  }

  void Start() {
    this->service_.Start();
  }

  void Stop() {
    this->service_.Stop();
  }

  bool Login(const std::string &name, ChatSession *session);
  bool Logout(const std::string &name);

  void BroadcastMessage(protocol::MessageType::type type,
      const char *message, size_t size);

  ChatSession *GetSession(const std::string &name) {
    ChatSessionMap::iterator iterator = this->sessions_.find(name);
    if(iterator != this->sessions_.end()) {
      return iterator->second;
    }
    return NULL;
  }

  inline global::ThriftPacket *GetThrift() {
    return &this->thrift_;
  }

  void OnDisconnect(ChatSession *session);

 private:
  void OnConnection(const coresh::ConnectionPtr &connection);

  coresh::ApplicationService service_;

  ChatSessionMap sessions_;
  ChatSessionSet connection_sessions_;

  global::ThriftPacket thrift_;
};

class ChatServerSingleton : public core::Noncopyable {
 public:
   ChatServerSingleton() {}
   ~ChatServerSingleton() {}


   static inline void SetInstance(ChatServer *server) {
     ChatServerSingleton::server_ = server;
   }
   static inline ChatServer &GetInstance() {
     return *ChatServerSingleton::server_;
   }

 private:
   static ChatServer *server_;
};

}  // namespace server

}  // namespace chat

}  // namespace test

#endif  // __TEST__CHAT__SERVER__CHAT__SERVER__H


