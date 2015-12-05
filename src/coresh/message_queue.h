//
// Summary: needart source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2012-12-27 10:36:09.
// File name: message_queue.h
//
// Description: 
// Define class MessageQueue.
//

#ifndef __CORESH__MESSAGE__QUEUE__H
#define __CORESH__MESSAGE__QUEUE__H

#include <boost/function.hpp>

#include "core/channel.h"
#include "core/event_pipe.h"
#include "core/base/bound_queue_common.h"
#include "core/base/noncopyable.h"
#include "core/base/time_tick.h"
#include "coresh/atomic_counter.h"
#include "coresh/lock_free_queue.h"

namespace coresh {

struct MessageQueueError {
  enum Type {
    ERROR_FULL = core::kErrorCodeFull,
    ERROR_EMPTY = core::kErrorCodeEmpty,
    ERROR_COMPLETE = core::kErrorCodeComplete,
    ERROR_SIZE = core::kErrorCodeErrorSize,
    ERROR_UNKNOWN = core::kErrorCodeUnknown,
  };
};

class MessageQueue : public core::Noncopyable {
  typedef boost::function<bool ()> ReadableCallback;
 public:
  MessageQueue(size_t initial_size, size_t max_message_size);
  ~MessageQueue();

  bool Allocate();
  void Deallocate();

  bool Initialize(core::EventLoop *reader);
  void Finalize();

  int Write(const char *data, size_t size);
  int Read(char *data, size_t size);

  bool FlushWrite();
  void Terminal();

  inline void SetReadableCallback(const ReadableCallback &callback) {
    this->readable_callback_ = callback;
  }
  inline void ResetReadableCallback() {
    this->readable_callback_.clear();
  }

 private:
  void OnReadCallback(const core::TimeTick &time_tick);
  void SendEvent();

  core::Channel event_channel_;
  int event_fd_[2];

  LockFreeQueue queue_;

  AtomicCounter event_counter_;

  ReadableCallback readable_callback_;
};

}  // namespace coresh

#endif  // __CORESH__MESSAGE__QUEUE__H

