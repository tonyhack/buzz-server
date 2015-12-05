//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-04 14:38:15.
// File name: event_handler.h
//
// Description:
// Define class EventHandler.
//

#ifndef __CORE__EVENT__HANDLER__H
#define __CORE__EVENT__HANDLER__H

#include <boost/function.hpp>

#include "core/base/noncopyable.h"

namespace core {

typedef boost::function<void ()> EventCallback;

class EventHandler : public Noncopyable {
 public:
  EventHandler() {}
  ~EventHandler() {}

  inline void SetEventCallback(const EventCallback &callback) {
    this->event_callback_ = callback;
  }

  void OnEventCallback() {
    this->event_callback_();
  }

 private:
  EventCallback event_callback_;
};

}

#endif  // __CORE__EVENT__HANDLER__H

