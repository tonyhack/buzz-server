//
// Summary: needart source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2012-12-27 10:36:12.
// File name: message_queue.cc
//
// Description: 
// Define class MessageQueue.
//

#include "coresh/message_queue.h"

#include <boost/bind.hpp>

#include "core/async_logging_singleton.h"

namespace coresh {

MessageQueue::MessageQueue(size_t initial_size, size_t max_message_size)
  : queue_(initial_size, max_message_size) {}

MessageQueue::~MessageQueue() {}

bool MessageQueue::Allocate() {
  core::CreateEventPipe(this->event_fd_);
  return true;
}

void MessageQueue::Deallocate() {
  core::CloseEventPipe(this->event_fd_);
}

bool MessageQueue::Initialize(core::EventLoop *reader) {
  this->event_counter_.Reset();
  this->queue_.Initialize();

  if(this->event_channel_.Initialize(reader,
        core::EventPipeReadfd(this->event_fd_)) == false) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Initialize Channel failed.", __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  this->event_channel_.SetReadCallback(
      boost::bind(&MessageQueue::OnReadCallback, this, _1));
  this->event_channel_.EnableRead();
  return true;
}

void MessageQueue::Finalize() {
  this->event_channel_.DisableRead();
  this->event_channel_.ResetReadCallback();
  this->event_channel_.Finalize();
  this->queue_.Finalize();
}

int MessageQueue::Write(const char *data, size_t size) {
  int retcode = this->queue_.Add(data, size);
  if(retcode > 0) {
    this->SendEvent();
  }
  return retcode;
}

int MessageQueue::Read(char *data, size_t size) {
  return this->queue_.Remove(data, size);
}

bool MessageQueue::FlushWrite() {
  if(this->queue_.Flush()) {
    this->SendEvent();
    return true;
  }
  return false;
}

void MessageQueue::Terminal() {
  this->queue_.Terminal();
  this->SendEvent();
}

void MessageQueue::OnReadCallback(const core::TimeTick &time_tick) {
  if(this->readable_callback_() == true) {
    core::uint64 data = 0;
    ssize_t size = ::read(core::EventPipeReadfd(this->event_fd_), &data, sizeof(data));
    if(size != sizeof(data)) {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) read from connect ready queue [%lld] bytes data.",
          __FILE__, __LINE__, __FUNCTION__, size);
    }
    this->event_counter_.Reset();
  }
}

void MessageQueue::SendEvent() {
  if(this->event_counter_.IncrementGet() <= 1) {
    core::uint64 data = 1;
    ssize_t size = ::write(core::EventPipeWritefd(this->event_fd_), &data, sizeof(data));
    if(size != sizeof(data)) {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) write to connect ready queue [%lld] bytes data.",
          __FILE__, __LINE__, __FUNCTION__, size);
    }
  }
}

}  // namespace coresh

