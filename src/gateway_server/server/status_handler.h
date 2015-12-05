//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-31 17:54:39.
// File name: status_handler.h
//
// Description:
// Define class StatusHandler.
//

#ifndef __GATEWAY__SERVER__STATUS__HANDLER__H
#define __GATEWAY__SERVER__STATUS__HANDLER__H

#include "core/base/noncopyable.h"
#include "gateway_server/server/check_session_queue.h"
#include "gateway_server/server/status_event.h"
#include "global/logging.h"

namespace gateway {

namespace server {

class GatewayUser;

struct UserStatus {
  enum Type {
    VERIFY_QUESTION_STATUS = 0,
    CHECK_SESSION_STATUS,
    LOGIN_STATUS,
    RUNNING_STATUS,
    LOGOUT_STATUS,

    MAX_STATUS,
  };
};

class StatusHandler : public core::Noncopyable {
 public:
  StatusHandler() : user_(NULL) {}
  virtual ~StatusHandler() {}

  virtual bool Initialize(GatewayUser *user) {
    if(user == NULL) {
      global::LogError("%s:%d (%s) Gateway user is null.",
          __FILE__, __LINE__, __FUNCTION__);
      return false;
    }
    this->user_ = user;
    return true;
  }

  virtual void Finalize() {
    this->user_ = NULL;
  }

  virtual UserStatus::Type GetStatus() = 0;

  virtual void Start() = 0;
  virtual void End() = 0;

  virtual void GotoNextStatus() = 0;

  virtual void OnMessage(const char *message, size_t size) = 0;
  virtual void OnEvent(StatusEventType::type type, const char *event, size_t size) {}

  virtual void OnDisconnect() {}

 protected:
  GatewayUser *user_;
};

}  // namespace server

}  // namespace gateway

#endif  // __GATEWAY__SERVER__STATUS__HANDLER__H

