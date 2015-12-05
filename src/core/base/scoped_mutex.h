//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-04 14:47:13.
// File name: scoped_mutex.h
//
// Description:
// ScopedMutex is an auto mutex in it's live socpe.
//

#ifndef __CORE__SCOPED__MUTEX__H
#define __CORE__SCOPED__MUTEX__H

namespace core {

template <typename MutexType>
class ScopedMutex {
 public:
  explicit ScopedMutex(MutexType *mutex) : mutex_(mutex){
    this->mutex_->Lock();
  }
  ~ScopedMutex() {
    this->mutex_->Unlock();
  }

 private:
  MutexType *mutex_;
};

}  // namespace core

#endif  // __CORE__SCOPED__MUTEX__H

