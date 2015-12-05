//
// Summary: needart source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2012-12-27 10:36:39.
// File name: nonblocking_queue.h
//
// Description: 
// Define class NonblockingQueue.
//

#ifndef __CORESH__NONBLOCKING__QUEUE__H
#define __CORESH__NONBLOCKING__QUEUE__H

#include <deque>

#include "core/base/auto_mutex.h"
#include "core/base/mutex.h"
#include "core/base/noncopyable.h"

namespace coresh {

template <typename T>
class NonblockingQueue : public core::Noncopyable {
  typedef std::deque<T> Queue;
 public:
  NonblockingQueue() {}
  ~NonblockingQueue() {}

  void Push(const T &e) {
    core::AutoMutex auto_mutex(&this->mutex_);
    this->queue_.push_back(e);
  }

  bool Pop(T &e) {
    core::AutoMutex auto_mutex(&this->mutex_);
    if(!this->queue_.empty()) {
      e = this->queue_.front();
      this->queue_.pop_front();
      return true;
    }
    return false;
  }

  inline size_t Size() const {
    return this->queue_.size();
  }

 private:
  Queue queue_;
  core::Mutex mutex_;
};

}  // namespace coresh

#endif  // __CORESH__NONBLOCKING__QUEUE__H

