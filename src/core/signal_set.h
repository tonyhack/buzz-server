//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-04 14:41:06.
// File name: signal_set.h
//
// Description:
// Define class SignalSet.
// This is not thread-safe, so uses it in a single thread.
//

#ifndef __CORE__SIGNAL__SET__H
#define __CORE__SIGNAL__SET__H

#include <signal.h>

#include <map>

#include "core/callbacks.h"
#include "core/base/mutex.h"
#include "core/base/noncopyable.h"

namespace core {

class Channel;
class EventLoop;
class Signaller;

class SignalSet : public Noncopyable {
  typedef std::map<int, Signaller> SignallerList;

 public:
  bool Initialize();

  bool Insert(int key, const SignalCallback &callback);
  // bool Remove(int key);

  void Run(int key);

  static SignalSet *GetSingleton() {
    static SignalSet signal_set;
    return &signal_set;
  }

  static void SignalHandler(int signum) {
    SignalSet::GetSingleton()->signal_handling_ = true;
    SignalSet::GetSingleton()->Run(signum);
    SignalSet::GetSingleton()->signal_handling_ = false;
  }

 protected:
  SignalSet();
  ~SignalSet();

 private:
  SignallerList signallers_;
  Mutex mutex_;
  bool signal_handling_;
};

}  // namespace core

#endif  // __CORE__SIGNAL__SET__H

