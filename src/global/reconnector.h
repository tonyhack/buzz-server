//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-21 10:44:29.
// File name: reconnector.h
//
// Description:
// Define class Reconnector.
//

#ifndef __GLOBAL__RECONNECTOR__H
#define __GLOBAL__RECONNECTOR__H

#include "core/loop_thread.h"
#include "core/base/noncopyable.h"
#include "global/reconnect_loop.h"

namespace global {

template <typename T>
class Reconnector : public core::Noncopyable {
 public:
  Reconnector() : thread_(&loop_, "ReconnectorThread", true) {}
  ~Reconnector() {}

  inline void Start() {
    this->thread_.StartLoop();
  }

  inline void Stop() {
    this->thread_.StopLoop(true);
  }

  inline void Reconnect(const T &request) {
    this->loop_.Reconnect(request);
  }

 private:
  ReconnectLoop<T> loop_;
  core::LoopThread thread_;
};

}  // namespace global

#endif  // __GLOBAL__RECONNECTOR__H

