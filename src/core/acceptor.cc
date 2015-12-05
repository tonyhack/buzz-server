//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-04 14:32:17.
// File name: acceptor.cc
//
// Description:
// Define class Acceptor.
// This acceptor uses non-blocking pattern.
//

#include "core/acceptor.h"

#include <boost/bind.hpp>

#include "core/inet_address.h"
#include "core/base/logging.h"

namespace core {

Acceptor::Acceptor()
       : loop_(NULL) {}

Acceptor::~Acceptor() {}

bool Acceptor::Initialize(const InetAddress &listen_address, EventLoop *loop) {
  if(loop == NULL) {
    CoreLog(ERROR, "%s:%d (%s) Invalid param loop.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->loop_ = loop;
  this->local_address_ = listen_address;

  if(this->socket_.CreateSocket() == false ||
         this->socket_.SetNonblock() == false ||
         this->socket_.Bind(this->local_address_) == false) {
    CoreLog(ERROR, "%s:%d (%s) Failed to create socket or bind address.",
           __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->channel_.Initialize(this->loop_, this->socket_.GetSockfd()) == false) {
    CoreLog(ERROR, "%s:%d (%s) Initialize Channel failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->channel_.SetReadCallback(boost::bind(&Acceptor::Accept, this, _1));

  return true;
}

void Acceptor::Finalize() {
  this->channel_.Finalize();

  this->loop_ = NULL;
}

bool Acceptor::Listen() {
  if(this->socket_.Listen(SOMAXCONN) == true) {
    this->channel_.EnableRead();
    return true;
  }
  return false;
}

void Acceptor::Accept(const TimeTick &time_tick) {
  CoreLog(INFO, "Acceptor::Accept()");
  InetAddress peer;
  int peer_sockfd = this->socket_.Accept(peer);
  if(peer_sockfd >= 0) {
    if(this->callback_)
      this->callback_(peer_sockfd, peer);
    else
      ::close(peer_sockfd);
  } else {
    CoreLog(ERROR, "%s:%d (%s) Failed to accept",
             __FILE__, __LINE__, __FUNCTION__);
  }
}

}  // namespace core

