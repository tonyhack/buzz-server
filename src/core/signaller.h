//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-04 14:40:53.
// File name: signaller.h
//
// Description:
// Define class Signaller.
//

#ifndef __CORE__SIGNALLER__H
#define __CORE__SIGNALLER__H

#include "core/callbacks.h"

namespace core {

class Signaller {
 public:
  explicit Signaller(const SignalCallback &callback) : number_(0), callback_(callback) {}
  ~Signaller() {}

  inline void Run() {
    this->callback_();
  }

  inline int GetNumber() const { return this->number_; }

 private:
  int number_;
  SignalCallback callback_;
};

}  // namespace core

#endif  // __CORE__SIGNALLER__H

