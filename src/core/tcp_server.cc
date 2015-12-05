//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-04 14:42:14.
// File name: tcp_server.cc
//
// Description:
// Define class TCPServer.
//

#include "core/tcp_server.h"

#include <boost/bind.hpp>

#include "core/event_loop.h"
#include "core/event_loop_thread_group.h"
#include "core/tcp_connection.h"

namespace core {

TCPServer::TCPServer(const InetAddress &address, const std::string &name)
  : name_(name), loop_(NULL), local_address_(address), connection_id_(0) {}

TCPServer::~TCPServer() {}

bool TCPServer::Initialize(EventLoop *loop, EventLoopThreadGroup *thread_group) {
  if(loop == NULL || thread_group == NULL) {
    CoreLog(ERROR, "Invalid params",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->loop_ = loop;
  this->thread_group_ = thread_group;

  if(this->acceptor_.Initialize(this->local_address_, this->loop_) == false) {
    CoreLog(ERROR, "Initialize Acceptor failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->acceptor_.SetNewConnectionCallback(boost::bind(&TCPServer::NewTCPConnection, this, _1, _2));

  return true;
}

void TCPServer::Finalize() {
  this->acceptor_.Finalize();
  this->connections_.clear();
}

bool TCPServer::Start() {
  if(this->acceptor_.Listen() == false) {
    CoreLog(ERROR, "%s:%d (%s) Server[%s] Listen error!!",
        __FILE__, __LINE__, __FUNCTION__, this->name_.c_str());
    return false;
  }
  CoreLog(INFO, "%s:%d (%s) Server[%s] start!!",
      __FILE__, __LINE__, __FUNCTION__, this->name_.c_str());
  return true;
}

void TCPServer::Stop() {}

void TCPServer::NewTCPConnection(int sockfd, const InetAddress &peer) {
  char buffer[16];
  snprintf(buffer, sizeof(buffer), ":%d:%u", sockfd, this->connection_id_++);
  std::string connection_name = this->name_ + buffer;
  TCPConnectionPtr connection(new (std::nothrow) TCPConnection(sockfd));
  if(connection == NULL) {
    CoreLog(ERROR, "%s:%d (%s) Server[%s] failed to allocate tcp connection.",
        __FILE__, __LINE__, __FUNCTION__, this->name_.c_str());
    return ;
  }
  if(connection->Initialize(this->thread_group_->GetNextLoop(),
        connection_name, this->local_address_, peer) == false) {
    CoreLog(ERROR, "%s:%d (%s) Server[%s] failed to initialize tcp connection.",
        __FILE__, __LINE__, __FUNCTION__, this->name_.c_str());
    return ;
  }
  ConnectionList::iterator iterator = this->connections_.find(connection_name);
  if(iterator == this->connections_.end()) {
    connection->SetConnectionCallback(this->connection_callback_);
    connection->SetMessageCallback(this->message_callback_);
    connection->SetWriteCompleteCallback(this->write_complete_callback_);
    connection->SetCloseCallback(boost::bind(&TCPServer::RemoveTCPConnection, this, _1));
    this->connections_.insert(std::make_pair(connection_name, connection));
    connection->ConnectionEstablished();
  } else {
    CoreLog(ERROR, "Connection exist");
  }
}

void TCPServer::RemoveTCPConnection(const TCPConnectionPtr &connection) {
  this->loop_->WakeupCallback(boost::bind(&TCPServer::RemoveInThreadLoop, this, connection), true);
}

void TCPServer::RemoveInThreadLoop(const TCPConnectionPtr &connection) {
  assert(connection);
  ConnectionList::iterator iterator = this->connections_.find(connection->GetName());
  if(iterator != this->connections_.end()) {
    assert(iterator->second);
    iterator->second->ConnectionDestoryed();
    iterator->second->Finalize();
    this->connections_.erase(iterator);
  } else {
    CoreLog(ERROR, "%s:%d (%s) Server[%s] failed to find connection[%s]",
        __FILE__, __LINE__, __FUNCTION__, this->name_.c_str(), connection->GetName().c_str());
  }
}

void TCPServer::RemoveAll() {
  ConnectionList::iterator iterator = this->connections_.begin();
  for(; iterator != this->connections_.end(); ++iterator) {
    assert(iterator->second);
    iterator->second->ConnectionDestoryed();
    iterator->second->Finalize();
  }
  this->connections_.clear();
}

}  // namespace core

