//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-04 14:40:39.
// File name: loop_thread.cc
//
// Description:
// Define class LoopThread.
//

#include "core/loop_thread.h"

#include <boost/bind.hpp>

#include "core/base/logging.h"

namespace core {

LoopThread::LoopThread(BaseLoop *loop, const std::string &name, bool joinable)
       : loop_(loop),
         thread_(name, boost::bind(&LoopThread::Loop, this), joinable) {}

LoopThread::~LoopThread() {}

void LoopThread::StartLoop() {
  this->thread_.Start();
}

void LoopThread::StopLoop(bool force_quit) {
  if(force_quit) this->GetLoop()->Quit();
  this->loop_->Wakeup();
  this->thread_.Join();
}

}  // namespace core

