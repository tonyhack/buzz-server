//
// Summary: needart source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2012-12-27 10:35:25.
// File name: blocking_queue.h
//
// Description: 
// Define class BlockingQueue.
//

#include "core/base/noncopyable.h"

#ifndef __CORESH__BLOCKING__QUEUE__H
#define __CORESH__BLOCKING__QUEUE__H

#include <deque>

#include "core/base/auto_mutex.h"
#include "core/base/condition_variable.h"
#include "core/base/mutex.h"
#include "core/base/noncopyable.h"

namespace coresh {

template <typename T>
class BlockingQueue : public core::Noncopyable {
  typedef std::deque<T> Queue;
 public:
  BlockingQueue() {}
  ~BlockingQueue() {}

  void Push(const T &e) {
    core::AutoMutex auto_mutex(&this->mutex_);
    this->queue_.push_back(e);
    this->condition_.Signal();
  }

  T Pop() {
    core::AutoMutex auto_mutex(&this->mutex_);
    while(this->queue_.empty()) {
      this->condition_.Wait(&this->mutex_);
    }
    T e = this->queue_.front();
    this->queue_.pop_front();
    return e;
  }

  size_t Size() {
    return this->queue_.size();
  }

 private:
  Queue queue_;

  core::Mutex mutex_;
  core::ConditionVariable condition_;
};

}  // namespace coresh

#endif  // __CORESH__BLOCKING__QUEUE__H

