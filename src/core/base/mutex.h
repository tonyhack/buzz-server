//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-04 14:46:53.
// File name: mutex.h
//
// Description:
// Define class Mutex.
//

#ifndef __CORE__MUTEX__H
#define __CORE__MUTEX__H

#include <pthread.h>
#include <string.h>

#include "core/base/noncopyable.h"

namespace core {

class Mutex : public Noncopyable {
 public:
  explicit Mutex(bool recursive = true) {
    int return_value = 0;
    if(recursive) { 
      pthread_mutexattr_t attr;
      return_value = pthread_mutexattr_init(&attr);
      if(return_value == 0) {
        return_value = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
        if(return_value == 0)
          return_value = pthread_mutex_init(&this->mutex_, &attr);
        // Destroy mutex attribute variable.
        return_value = pthread_mutexattr_destroy(&attr);
      }
    } else {
      // Defualt is "PTHREAD_MUTEX_DEFAULT".
      return_value = pthread_mutex_init(&this->mutex_, NULL);
    }

  }
  ~Mutex() { pthread_mutex_destroy(&this->mutex_); }

  inline void Lock() {
    pthread_mutex_lock(&this->mutex_);
  }
  inline void Unlock() {
    pthread_mutex_unlock(&this->mutex_);
  }

 private:
  pthread_mutex_t mutex_;

  friend class ConditionVariable;
};

}  // namespace core

#endif  // __CORE__MUTEX__H

