//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-04 14:40:32.
// File name: loop_thread.h
//
// Description:
// Define class LoopThread.
//

#ifndef __CORE__LOOP__THREAD__H
#define __CORE__LOOP__THREAD__H

#include <string>

#include "core/base_loop.h"
#include "core/base/noncopyable.h"
#include "core/base/thread.h"

namespace core {

class LoopThread : public Noncopyable {
 public:
  LoopThread(BaseLoop *loop, const std::string &name = "", bool joinable = true);
  ~LoopThread();

  void StartLoop();
  void StopLoop(bool force_quit = true);
  inline BaseLoop *GetLoop() const { return this->loop_; }

 private:
  inline void Loop() { this->loop_->Loop(); }

  BaseLoop *loop_;
  Thread thread_;
};

}  // namespace core


#endif  // __CORE__LOOP__THREAD__H

