//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-02-22 14:56:58.
// File name: status_login_handler.h
//
// Description:
// Define class StatusLoginHandler.
//

#ifndef __GATEWAY__SERVER__STATUS__LOGIN__HANDLER__H
#define __GATEWAY__SERVER__STATUS__LOGIN__HANDLER__H

#include "core/timer_handler.h"
#include "core/base/types.h"
#include "gateway_server/server/status_handler.h"

namespace gateway {

namespace server {

class GatewayUser;

class StatusLoginHandler : public StatusHandler, public core::TimerHandler {
  enum { TIMER_LOGIN_TIMEOUT = 0, };
  enum { LOGIN_TIMEOUT_SECS = 10, };

 public:
  StatusLoginHandler();
  virtual ~StatusLoginHandler();

  virtual bool Initialize(GatewayUser *user);
  virtual void Finalize();

  virtual UserStatus::Type GetStatus();

  virtual void Start();
  virtual void End();

  virtual void GotoNextStatus();

  virtual void OnMessage(const char *message, size_t size);
  virtual void OnEvent(StatusEventType::type type, const char *event, size_t size);

  virtual void OnTimer(core::uint32 id);

  virtual void OnDisconnect();

 private:
  void SendLogoutSession();
  void OnRequestUserLogin();
  void OnUserLoginFinish(const char *message, size_t size);
};

}  // namespace server

}  // namespace gateway

#endif  // __GATEWAY__SERVER__STATUS__LOGIN__HANDLER__H

