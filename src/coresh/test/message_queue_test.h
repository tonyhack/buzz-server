//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-05 10:54:26.
// File name: message_queue_test.h
//
// Description:
// Define class MessageQueueTest.
//

#ifndef __CORESH__TEST__MESSAGE__QUEUE__TEST__H
#define __CORESH__TEST__MESSAGE__QUEUE__TEST__H

#include "core/event_loop.h"
#include "core/loop_thread.h"
#include "core/base/noncopyable.h"
#include "coresh/message_queue.h"

namespace coresh {

namespace test {

class MessageQueueTest : public core::Noncopyable {
 public:
  MessageQueueTest() : thread_(&loop_), queue_(1024, 32) {}
  ~MessageQueueTest() {}

  bool Initialize();
  void Start();

  void SendMessage(const char *message, size_t size);
  bool ReceiveMessage();

 private:
  core::EventLoop loop_;
  core::LoopThread thread_;

  MessageQueue queue_;
};

}  // namespace test

}  // namespace coresh

#endif  // __CORESH__TEST__MESSAGE__QUEUE__TEST__H

