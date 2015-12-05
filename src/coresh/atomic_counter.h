//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2012-12-27 10:35:17.
// File name: atomic_counter.h
//
// Description: 
// Define class AtomicCounter.
//

#ifndef __CORESH__ATOMIC__COUNTER__H
#define __CORESH__ATOMIC__COUNTER__H

#include "core/base/types.h"
#include "core/base/noncopyable.h"

namespace coresh {

class AtomicCounter : core::Noncopyable {
 public:
  AtomicCounter() : counter_(0) {}
  ~AtomicCounter() {}

  inline core::uint32 IncrementGet() {
    return __sync_fetch_and_add(&this->counter_, 1);
  }
  inline core::uint32 DecrementGet() {
    return __sync_fetch_and_add(&this->counter_, -1);
  }

  inline core::uint32 Get() {
    return __sync_val_compare_and_swap(&this->counter_, 0, 0);
  }
  inline core::uint32 Reset() {
    return __sync_lock_test_and_set(&this->counter_, 0);
  }

 private:
  core::uint32 counter_;
};

}  // namespace coresh

#endif  // __CORESH__ATOMIC__COUNTER__H

