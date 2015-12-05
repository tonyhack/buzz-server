//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-04 14:39:19.
// File name: event_loop_thread_group.h
//
// Description:
// Define class EventLoopThreadGroup.
//

#ifndef __CORE__EVENT__LOOP__THREAD__GROUP__H
#define __CORE__EVENT__LOOP__THREAD__GROUP__H

#include <string>
#include <vector>

#include "core/base/auto_mutex.h"
#include "core/base/mutex.h"
#include "core/base/noncopyable.h"

namespace core {

class LoopThread;
class EventLoop;

class EventLoopThreadGroup : public Noncopyable {
 public:
  typedef std::vector<EventLoop *> LoopGroup;
  typedef std::vector<LoopThread *> ThreadGroup;

  EventLoopThreadGroup();
  ~EventLoopThreadGroup();

  bool Initialize(size_t thread_number, const std::string &name = "", bool timer = false);
  void Finalize();

  void Start();
  void Stop();

  inline EventLoop *GetNextLoop() {
    AutoMutex auto_mutex(&this->mutex_);
    return this->loops_[this->hash_++ % this->loops_.size()];
  }

  inline const LoopGroup *GetLoopGroup() const {
    return &this->loops_;
  }

 private:
  ThreadGroup threads_;
  LoopGroup loops_;
  size_t hash_;
  Mutex mutex_;
};

}  // namespace core

#endif  // __CORE__EVENT__LOOP__THREAD__GROUP__H

