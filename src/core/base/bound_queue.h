//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-04 14:44:47.
// File name: bound_queue.h
//
// Description:
// Define class BoundQueue.
// Muti-thread safe.
//

#ifndef __CORE__BOUND__QUEUE__H
#define __CORE__BOUND__QUEUE__H

#include "core/base/auto_mutex.h"
#include "core/base/bound_queue_common.h"
#include "core/base/byte_buffer_static.h"
#include "core/base/condition_variable.h"
#include "core/base/mutex.h"
#include "core/base/noncopyable.h"

namespace core {

class BoundQueue : public Noncopyable {
 public:
  BoundQueue(size_t max_size, int blocking_pattern);
  ~BoundQueue();

  int Add(const char *data, size_t size);
  int Remove(char *data, size_t size);

  inline void Terminate() {
    this->terminate_flag_ = true;
    this->WakeupProducer();
    this->WakeupConsumer();
  }
  inline bool CheckTerminate() const {
    return this->terminate_flag_;
  }

 private:
  inline void WakeupProducer() {
    if(this->blocking_pattern_ & BLOCKING_PRODUCER) {
      AutoMutex auto_mutex(&this->mutex_not_full_);
      this->condition_not_full_.Broadcast();
    }
  }
  inline void WakeupConsumer() {
    if(this->blocking_pattern_ & BLOCKING_CONSUMER) {
      AutoMutex auto_mutex(&this->mutex_not_empty_);
      this->condition_not_empty_.Broadcast();
    }
  }

  // The queue.
  ByteBufferStaticMalloc queue_;
  size_t size_;

  volatile size_t write_pointer_;
  volatile size_t read_pointer_;

  mutable Mutex mutex_not_empty_;
  mutable Mutex mutex_not_full_;
  ConditionVariable condition_not_empty_;
  ConditionVariable condition_not_full_;

  int blocking_pattern_;

  bool terminate_flag_;
};

}  // namespace core

#endif  // __CORE__BOUND__QUEUE__H

