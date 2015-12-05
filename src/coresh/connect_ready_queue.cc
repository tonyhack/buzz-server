//
// Summary: needart source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2012-12-27 10:35:52.
// File name: connect_ready_queue.cc
//
// Description: 
// Define class ConnectReadyQueue.
//

#include "coresh/connect_ready_queue.h"

#include <boost/bind.hpp>

#include "core/async_logging_singleton.h"

namespace coresh {

ConnectReadyQueue::ConnectReadyQueue() {
  core::CreateEventPipe(this->event_fd_);
}

ConnectReadyQueue::~ConnectReadyQueue() {
  core::CloseEventPipe(this->event_fd_);
}

bool ConnectReadyQueue::Initialize(core::EventLoop *receiver) {
  if(receiver == NULL) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Invalid param.", __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->event_channel_.Initialize(receiver,
        core::EventPipeReadfd(this->event_fd_)) == false) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Initialize Channel failed.", __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->event_channel_.SetReadCallback(
      boost::bind(&ConnectReadyQueue::OnReadCallback, this, _1));
  this->event_channel_.EnableRead();

  return true;
}

void ConnectReadyQueue::Finalize() {
  this->event_channel_.ResetReadCallback();
  this->event_channel_.DisableRead();

  this->event_channel_.Finalize();
}

void ConnectReadyQueue::OnReceive(const ConnectionPtr &connection) {
  this->queue_.Push(connection);
  this->SendEvent();
}

void ConnectReadyQueue::OnReadCallback(const core::TimeTick &time_tick) {
  core::uint64 data = 0;
  ssize_t size = ::read(core::EventPipeReadfd(this->event_fd_), &data, sizeof(data));
  if(size != sizeof(data)) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) read from connect ready queue [%lld] bytes data.",
        __FILE__, __LINE__, __FUNCTION__, size);
  }

  ConnectionPtr connection;
  while(this->queue_.Pop(connection)) {
    this->connect_callback_(connection);
  }
}

void ConnectReadyQueue::SendEvent() {
    core::uint64 data = 1;
    ssize_t size = ::write(core::EventPipeWritefd(this->event_fd_), &data, sizeof(data));
    if(size != sizeof(data)) {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) write to connect ready queue [%lld] bytes data.",
          __FILE__, __LINE__, __FUNCTION__, size);
    }
}

}  // namespace coresh

