//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-21 10:26:26.
// File name: reconnect_loop.h
//
// Description:
// Define class ReconnectLoop.
//

#ifndef __GLOBAL__RECONNECT__LOOP__H
#define __GLOBAL__RECONNECT__LOOP__H

#include "core/base_loop.h"
#include "coresh/blocking_queue.h"

namespace global {

template <typename T>
class ReconnectLoop : public core::BaseLoop {
 public:
  ReconnectLoop() {}
  virtual ~ReconnectLoop() {}

  virtual void Loop() {
    static const useconds_t kMaxUsecs = 1000 * 1000;
    static const useconds_t kMinUsecs = 20 * 1000;
    useconds_t usecs = 0;

    this->looping_ = true;

    while(this->quit_ == false) {
      T element = this->queue_.Pop();
      // Check quit.
      if(element.CheckValid()) {
        if(element.Reconnect() == false) {
          // Reconnect failed, push it.
          this->queue_.Push(element);
          usecs = kMaxUsecs / this->queue_.Size();
          if(usecs < kMinUsecs) usecs = kMinUsecs;
          usleep(usecs);
        }
      } else {
        this->quit_ = true;
      }
    }

    this->looping_ = false;
  }

  virtual void Wakeup() {
    T element;
    element.SetValid(false);
    this->queue_.Push(element);
  }

  void Reconnect(const T &request) {
    this->queue_.Push(request);
  }

 private:
  coresh::BlockingQueue<T> queue_;
};

}  // namespace global

#endif  // __GLOBAL__RECONNECT__LOOP__H

