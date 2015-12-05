//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-04 14:37:55.
// File name: event_control.cc
//
// Description:
// Define class EventControl.
//

#include <boost/bind.hpp>

#include "core/event_control.h"
#include "core/event_handler.h"
#include "core/event_loop.h"
#include "core/event_pipe.h"

namespace core {

EventControl::EventControl() {}
EventControl::~EventControl() {}

bool EventControl::Initialize(EventLoop *loop) {
  if(loop == NULL) {
    CoreLog(ERROR, "%s:%d (%s) Invalid param loop.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->loop_ = loop;

  CreateEventPipe(this->fd_);

  if(this->channel_.Initialize(this->loop_, EventPipeReadfd(this->fd_)) == false) {
    CoreLog(ERROR, "%s:%d (%s) Initialize Channel failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  return true;
}

void EventControl::Finalize() {
  this->channel_.DisableAll();
  ::close(this->channel_.GetFd());

  this->channel_.Finalize();
}


bool EventControl::Start() {
  this->channel_.SetReadCallback(boost::bind(&EventControl::OnReadCallback, this, _1));
  this->channel_.EnableRead();

  return true;
}

void EventControl::SetEvent(const EventHandler *handler) {
  assert(handler);
  ssize_t n = ::write(EventPipeWritefd(this->fd_), (uint64 *)handler, sizeof(uint64));
  if(n != sizeof(uint64)) {
    CoreLog(ERROR, "%s:%d (%s) write [%u] bytes data.",
          __FILE__, __LINE__, __FUNCTION__, n);
  }
}

void EventControl::OnReadCallback(const TimeTick &time_tick) {
  uint64 data = 0;
  ssize_t n = ::read(EventPipeReadfd(this->fd_), &data, sizeof(data));
  if(n == sizeof(data)) {
    EventHandler *handler = (EventHandler *)data;
    assert(handler);
    handler->OnEventCallback();
  } else {
    CoreLog(ERROR, "%s:%d (%s) wakeup and read [%u] bytes data.",
           __FILE__, __LINE__, __FUNCTION__, n);
  }
}

}  // namespace core

