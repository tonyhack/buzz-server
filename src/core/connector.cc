//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-04 14:36:20.
// File name: connector.cc
//
// Description:
// Define class Connector.
// This Connector uses non-blocking pattern and must run in a EventLoop.
//

#include "core/connector.h"

#include <errno.h>

#include <boost/bind.hpp>

#include "core/event_loop.h"
#include "core/base/logging.h"

namespace core {

Connector::Connector() {}
Connector::~Connector() {}

bool Connector::Initialize(EventLoop *loop, const InetAddress &server_address,
    int retry_msec) {
  if(loop == NULL) {
    CoreLog(ERROR, "%s:%d (%s) Invalid param loop",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->channel_.Initialize(loop) == false) {
    CoreLog(ERROR, "%s:%d (%s) Initialize Channel failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->loop_ = loop;
  this->address_ = server_address;
  this->state_ = DISCONNECTED;
  this->retry_msec_ = retry_msec;
  this->retry_flag_ = false;

  return true;
}

void Connector::Finalize() {
  if(this->retry_msec_ > 0 && this->retry_flag_ == true) {
    this->loop_->RemoveTimer(1, this);
    this->retry_flag_ = false;
  }

  this->socket_.Close();

  this->channel_.Finalize();

  this->loop_ = NULL;
}

void Connector::OnTimer(uint32 id) {
  assert(this->loop_);
  this->loop_->RemoveTimer(1, this);
  this->retry_flag_ = false;

  this->Start();
}

void Connector::Start() {
  if(this->state_ == DISCONNECTED) {
    // To connect.
    this->Connect();
    this->state_ = CONNECTING;
  } else {
    CoreLog(ERROR, "%s:%d (%s) Failed to start to connect, state is error.",
           __FILE__, __LINE__, __FUNCTION__);
  }
}

void Connector::Stop() {
  this->socket_.Close();
  this->state_ = DISCONNECTED;
}

void Connector::Restart() {
  this->Stop();
  if(this->retry_msec_ > 0) {
    this->state_ = DISCONNECTED;
    if(this->retry_flag_ == true) {
      this->loop_->RemoveTimer(1, this);
    }
    this->loop_->AddTimer(1, this->retry_msec_, 1, this, __FUNCTION__);
    this->retry_flag_ = true;
  } else {
    CoreLog(ERROR, "%s:%d (%s) Failed to restart, retry_msec_=[%d].",
           __FILE__, __LINE__, __FUNCTION__, this->retry_msec_);
  }
}

void Connector::Connect() {
  this->socket_.Close();
  this->channel_.DisableAll();
  if(this->socket_.CreateSocket() == true && this->socket_.SetNonblock() == true) {
    if(this->socket_.Connect(this->address_) == true) {
      this->channel_.SetFd(this->socket_.GetSockfd());
      this->channel_.SetWriteCallback(boost::bind(&Connector::OnComplete, this));
      this->channel_.SetErrorCallback(boost::bind(&Connector::OnError, this));
      this->channel_.EnableWrite();
    } else {
      // TODO: Retry timeout.
      this->Restart();
      CoreLog(ERROR, "%s:%d (%s) Failed to connect to address(%s)",
          __FILE__, __LINE__, __FUNCTION__, this->address_.ToString().c_str());
    }
  } else {
    CoreLog(ERROR, "%s:%d (%s) Failed to create socket or set noblock.",
        __FILE__, __LINE__, __FUNCTION__);
  }
}

void Connector::RemoveChannel() {
  this->channel_.DisableAll();
}

void Connector::DelayRemoveChannel() {
  this->loop_->WakeupCallback(boost::bind(&Connector::RemoveChannel, this), true);
}

void Connector::OnComplete() {
  if(this->state_ == CONNECTING) {
    CoreLog(INFO, "%s:%d (%s) Connect ok", __FILE__, __LINE__, __FUNCTION__);
    // Remove the retry timer.
    if(this->retry_msec_ > 0 && this->retry_flag_ == true) {
      this->loop_->RemoveTimer(1, this);
      this->retry_flag_ = false;
    }
    this->DelayRemoveChannel();
    this->state_ = CONNECTED;
    // Get peer address.
    struct sockaddr_in local_address;
    memset(&local_address, 0, sizeof(local_address));
    socklen_t addrlen = sizeof(local_address);
    if(::getsockname(this->socket_.GetSockfd(), (struct sockaddr *)(&local_address), &addrlen) < 0)
    {
      CoreLog(ERROR, "%s:%d (%s) Failed to getsockname", __FILE__, __LINE__, __FUNCTION__);
    } else {
      if(this->callback_) {
        this->callback_(this->socket_.GetSockfd(), InetAddress(local_address));
      }
    }
  }
}

void Connector::OnError() {
  int optval;
  socklen_t optlen = sizeof(optval);
  if (::getsockopt(this->socket_.GetSockfd(), SOL_SOCKET, SO_ERROR, &optval, &optlen) < 0)
    optval = errno;

  CoreLog(ERROR, "%s:%d (%s) Error occur error(%d) address(%s)",
       __FILE__, __LINE__, __FUNCTION__, optval, this->address_.ToString().c_str());

  this->DelayRemoveChannel();

  this->Restart();
}

}  // namespace core

