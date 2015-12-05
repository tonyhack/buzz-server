//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2012-12-27 10:35:59.
// File name: lock_free_queue.h
//
// Description: 
// Define class LockFreeQueue.
//

#ifndef __CORESH__LOCK__FREE__QUEUE__H
#define __CORESH__LOCK__FREE__QUEUE__H

#include "core/base/bound_queue_common.h"
#include "core/base/byte_buffer_dynamic.h"
#include "core/base/byte_buffer_static.h"
#include "core/base/noncopyable.h"

namespace coresh {

class LockFreeQueue : public core::Noncopyable {
 public:
  LockFreeQueue(size_t max_size, size_t max_unit_size);
  ~LockFreeQueue();

  inline void Initialize() {
    this->write_pointer_ = 0;
    this->read_pointer_ = 0;
    this->terminal_ = false;
    this->dynamic_queue_.Initialize();
  }
  inline void Finalize() {
    this->write_pointer_ = 0;
    this->read_pointer_ = 0;
    this->terminal_ = false;
    this->dynamic_queue_.Finalize();
  }

  int Add(const char *data, size_t size);
  int Remove(char *data, size_t max_size);

  inline bool CheckEmpty() const {
    return this->write_pointer_ == this->read_pointer_ ||
      this->dynamic_queue_.ReadSize() == 0;
  }

  inline void Terminal() { this->terminal_ = true; }
  inline bool CheckTerminal() const { return this->terminal_; }
  inline bool Flush() { return this->Shift2Ring(); }

 private:
  bool Shift2Ring();

  bool TryAdd(const char *data, size_t size);

  core::ByteBufferStaticMalloc queue_;
  size_t size_;

  volatile size_t write_pointer_;
  volatile size_t read_pointer_;

  core::ByteBufferDynamic dynamic_queue_;

  volatile bool terminal_;
};

}  // namespace coresh

#endif  // __CORESH__LOCK__FREE__QUEUE__H

