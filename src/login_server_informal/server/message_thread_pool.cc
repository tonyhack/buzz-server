//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-05 14:23:28.
// File name: message_thread_pool.cc
//
// Description:
// Define class MessageThreadPool.
//

#include "login_server_informal/server/message_thread_pool.h"

#include "global/logging.h"

namespace login {

namespace server {

MessageThreadPool::MessageThreadPool() {}
MessageThreadPool::~MessageThreadPool() {}

bool MessageThreadPool::Initialize(size_t thread_number,
    const std::string &name, bool timer) {
  if(this->thread_pool_.Initialize(thread_number, name, timer) == false) {
    global::LogError("%s:%d (%s) Initialize EventLoopThreadGroup error.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  const core::EventLoopThreadGroup::LoopGroup *group = this->thread_pool_.GetLoopGroup();
  if(group == NULL) {
    global::LogError("%s:%d (%s) GetLoopGroup error.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  core::EventLoopThreadGroup::LoopGroup::const_iterator iterator = group->begin();
  for(; iterator != group->end(); ++iterator) {
    // ThriftPacket.
    global::ThriftPacketPtr packet(new (std::nothrow) global::ThriftPacket());
    if(packet.get() == NULL) {
      global::LogError("%s:%d (%s) Allocate ThriftPacket error.",
          __FILE__, __LINE__, __FUNCTION__);
      return false;
    }
    this->packets_.insert(std::make_pair(*iterator, packet));

    // MysqlClient.
    MysqlClientPtr mysql(new (std::nothrow) MysqlClient());
    if(mysql.get() == NULL) {
      global::LogError("%s:%d (%s) Allocate MysqlClient error.",
          __FILE__, __LINE__, __FUNCTION__);
      return false;
    }
    if(mysql->Initialize() == false) {
      global::LogError("%s:%d (%s) Initialize MysqlClient error.",
          __FILE__, __LINE__, __FUNCTION__);
      return false;
    }
    this->mysqls_.insert(std::make_pair(*iterator, mysql));
  }

  return true;
}

void MessageThreadPool::Start() {
  MysqlMap::iterator iterator = this->mysqls_.begin();
  for(; iterator != this->mysqls_.end(); ++iterator) {
    if(iterator->second->Start() == false) {
      global::LogError("%s:%d (%s) Start mysql failed, this cause abort.",
          __FILE__, __LINE__, __FUNCTION__);
      ::abort();
    }
  }
  this->thread_pool_.Start();
}

void MessageThreadPool::Stop() {
  this->thread_pool_.Stop();
  MysqlMap::iterator iterator = this->mysqls_.begin();
  for(; iterator != this->mysqls_.end(); ++iterator) {
    iterator->second->Stop();
  }
}

}  // namespace server

}  // namespace login

