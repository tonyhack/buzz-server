//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-01-07 16:04:43.
// File name: boundless_queue.h
//
// Description:
// Define class BoundlessQueue.
// Muti-thread safe.
//

#ifndef __CORE__BOUNDLESS__QUEUE__H
#define __CORE__BOUNDLESS__QUEUE__H

#include "core/base/auto_mutex.h"
#include "core/base/bound_queue_common.h"
#include "core/base/byte_buffer_dynamic.h"
#include "core/base/byte_buffer_static.h"
#include "core/base/condition_variable.h"
#include "core/base/mutex.h"
#include "core/base/noncopyable.h"

namespace core {

class BoundlessQueue : public Noncopyable {
 public:
  BoundlessQueue(size_t queue_size, int blocking_pattern, int reduce_waterline = 0);
  ~BoundlessQueue();

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
  inline void WakeupProducer() {}
  inline void WakeupConsumer() {
    if(this->blocking_pattern_ & BLOCKING_CONSUMER) {
      AutoMutex auto_mutex(&this->write_mutex_);
      this->condition_not_empty_.Broadcast();
    }
  }

  bool Shift2Ring();
  bool TryAdd(const char *data, size_t size);
  int TryRemove(char *data, size_t size);

  // The queue.
  ByteBufferStaticMalloc queue_;
  size_t size_;

  volatile size_t write_pointer_;
  volatile size_t read_pointer_;

  ByteBufferDynamic dynamic_queue_;

  Mutex write_mutex_;
  Mutex read_mutex_;
  Mutex dynamic_mutex_;
  ConditionVariable condition_not_empty_;

  int blocking_pattern_;

  bool terminate_flag_;

  int reduce_waterline_;

  int shift_success_count_;
};

}  // namespace core

#endif  // __CORE__BOUNDLESS__QUEUE__H

