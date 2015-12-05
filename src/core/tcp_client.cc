//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-04 14:41:38.
// File name: tcp_client.cc
//
// Description:
// Define class TCPClient.
//

#include "core/tcp_client.h"

#include <boost/bind.hpp>

#include "core/configure.h"
// #include "core/tcp_connection.h"
#include "core/base/auto_mutex.h"
#include "core/base/logging.h"

namespace core {

TCPClient::TCPClient()
  : connected_(false), retry_(true), loop_(NULL) {}

TCPClient::~TCPClient() {}

bool TCPClient::Initialize(const InetAddress &server_address, const std::string &name,
    EventLoop *loop, bool retry) {
  if(loop == NULL) {
    CoreLog(ERROR, "%s:%d (%s) Invalid param loop.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->connector_.Initialize(loop, server_address,
        Configure::GetConnector()->retry_msec_) == false) {
    CoreLog(ERROR, "%s:%d (%s) Initialize Connector failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->connector_.SetNewConnectionCallback(
      boost::bind(&TCPClient::NewTCPConnection, this, _1, _2));

  this->server_address_ = server_address;
  this->name_ = name;
  this->loop_ = loop;
  this->retry_ = retry;

  return true;
}

void TCPClient::Finalize() {
  {
    AutoMutex auto_mutex(&this->mutex_);
    if(this->connection_) {
      this->connection_->Finalize();
      this->connection_.reset();
      CoreLog(INFO, "Finalize reset connection_");
    }
  }

  this->connector_.Finalize();
}

void TCPClient::Start() {
  if(this->connected_ == false) {
    this->connector_.Start();
  }
}

void TCPClient::Stop() {
  this->retry_ = false;
  this->connector_.Stop();
  if(this->connected_ == true) {
    AutoMutex auto_mutex(&this->mutex_);
    if(this->connection_)
      this->connection_->Shutdown();
  }
}

void TCPClient::SendMessage(const char *message, size_t size) {
  if(this->connection_) {
    this->connection_->SendMessage(message, size);
  }
}

void TCPClient::NewTCPConnection(int sockfd, const InetAddress &local) {
  char buffer[16];
  snprintf(buffer, sizeof(buffer), ":%d", sockfd);
  std::string connection_name = this->name_ + buffer;
  this->connection_.reset(new (std::nothrow) TCPConnection(sockfd));
  if(this->connection_ == NULL) {
    CoreLog(ERROR, "%s:%d (%s) Client[%s] failed to allocate tcp connection.",
        __FILE__, __LINE__, __FUNCTION__, this->name_.c_str());
    return ;
  }
  if(this->connection_->Initialize(this->loop_,
      connection_name, local, this->server_address_) == false) {
    CoreLog(ERROR, "%s:%d (%s) Client[%s] failed to initialize tcp connection.",
        __FILE__, __LINE__, __FUNCTION__, this->name_.c_str());
    return ;
  }
  this->connection_->SetConnectionCallback(this->connection_callback_);
  this->connection_->SetMessageCallback(this->message_callback_);
  this->connection_->SetWriteCompleteCallback(this->write_complete_callback_);
  this->connection_->SetCloseCallback(boost::bind(&TCPClient::RemoveTCPConnection, this, _1));
  this->connection_->ConnectionEstablished();
  this->connected_ = true;
}

// void TCPClient::RemoveTCPConnection(const TCPConnection *connection) {
void TCPClient::RemoveTCPConnection(const TCPConnectionPtr &connection) {
  this->connected_ = false;
  CoreLog(ERROR, "RemoveTCPConnection %p", this->connection_.get());
  if(this->connection_.get() != NULL) {
    AutoMutex auto_mutex(&this->mutex_);
    this->connection_->ConnectionDestoryed();
    // delete this->connection_;
    // this->connection_ = NULL;
    this->connection_->Finalize();
    this->connection_.reset();
    CoreLog(INFO, "RemoveTCPConnection reset connection_");
  }
  if(this->retry_) {
    this->connector_.Restart();
  }
}

}  // namespace core

