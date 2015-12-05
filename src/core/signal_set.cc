//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-04 14:41:14.
// File name: signal_set.cc
//
// Description:
// Define class SignalSet.
//

#include "core/signal_set.h"

#include <boost/bind.hpp>

#include "core/signaller.h"
#include "core/base/auto_mutex.h"
#include "core/base/logging.h"

namespace core {

SignalSet::SignalSet() : signal_handling_(false) {}

SignalSet::~SignalSet() {}

bool SignalSet::Insert(int key, const SignalCallback &callback) {
  if(this->signal_handling_ == true) {
    CoreLog(ERROR, "%s:%d (%s) Failed to insert when signal handled.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  AutoMutex auto_mutex(&this->mutex_);
  if(this->signallers_.find(key) == this->signallers_.end()) {
    if(::signal(key, SignalSet::SignalHandler) == SIG_ERR) {
      CoreLog(ERROR, "%s:%d (%s) Failed to signal, key=[%d].",
          __FILE__, __LINE__, __FUNCTION__, key);
      return false;
    }
    // Insert to list.
    Signaller signaller(callback);
    this->signallers_.insert(std::make_pair(key, signaller));
    return true;
  }

  return false;
}
/*
bool SignalSet::Remove(int key) {
  if(this->signal_handling_ == true) {
    CoreLog(ERROR, "%s:%d (%s) Failed to insert when signal handled.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  AutoMutex auto_mutex(&this->mutex_);
  SignallerList::iterator iterator = this->signallers_.find(key);
  if(iterator != this->signallers_.end()) {
    // Unblock signal.
    sigset_t mask;
    ::sigemptyset(&mask);
    ::sigaddset(&mask, key);
    if(::sigprocmask(SIG_UNBLOCK, &mask, NULL) == -1) {
      CoreLog(ERROR, "%s:%d (%s) Failed to sigprocmask, key=[%d].",
             __FILE__, __LINE__, __FUNCTION__, key);
      return false;
    }
    // Remove from list.
    this->signallers_.erase(iterator);
    return true;
  }

  return false;
}
*/
void SignalSet::Run(int key) {
  AutoMutex auto_mutex(&this->mutex_);
  SignallerList::iterator iterator = this->signallers_.find(key);
  if(iterator != this->signallers_.end()) {
    iterator->second.Run();
  }
}

}  // namespace core

