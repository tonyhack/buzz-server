//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-04 14:39:26.
// File name: event_loop_thread_group.cc
//
// Description:
// Define class EventLoopThreadGroup.
//

#include "core/event_loop_thread_group.h"

#include "core/event_loop.h"
#include "core/loop_thread.h"
#include "core/base/logging.h"

namespace core {

EventLoopThreadGroup::EventLoopThreadGroup() : hash_(0) {}
EventLoopThreadGroup::~EventLoopThreadGroup() {}

bool EventLoopThreadGroup::Initialize(size_t thread_number, const std::string &name, bool timer) {
  size_t pos = 0;
  for(; pos < thread_number; ++pos) {
    EventLoop *loop = new EventLoop();
    if(loop == NULL) {
      CoreLog(ERROR, "%s:%d (%s) Failed in allocate EventLoop.", __FILE__, __LINE__, __FUNCTION__);
      return false;
    }
    if(loop->Initialize(timer) == false) {
      CoreLog(ERROR, "%s:%d (%s) Failed in EventLoop's initialize.", __FILE__, __LINE__, __FUNCTION__);
      delete loop;
      return false;
    }
    this->loops_.push_back(loop);

    LoopThread *thread = new LoopThread(loop, name, true);
    if(thread == NULL) {
      CoreLog(ERROR, "%s:%d (%s) Failed in allocate LoopThread.", __FILE__, __LINE__, __FUNCTION__);
      return false;
    }
    this->threads_.push_back(thread);
  }

  return true;
}

void EventLoopThreadGroup::Finalize() {
  for(ThreadGroup::iterator iterator = this->threads_.begin(); 
         iterator != this->threads_.end(); ++iterator) {
    delete (*iterator);
  }

  for(LoopGroup::iterator iterator = this->loops_.begin(); 
         iterator != this->loops_.end(); ++iterator) {
    (*iterator)->Finalize();
    delete (*iterator);
  }

  this->threads_.clear();
  this->loops_.clear();
}

void EventLoopThreadGroup::Start() {
  ThreadGroup::iterator iterator = this->threads_.begin();
  for(; iterator != this->threads_.end(); ++iterator) {
    (*iterator)->StartLoop();
  }
}

void EventLoopThreadGroup::Stop() {
  ThreadGroup::iterator iterator = this->threads_.begin();
  for(; iterator != this->threads_.end(); ++iterator) {
    (*iterator)->StopLoop();
  }
}

}  // namespace core

