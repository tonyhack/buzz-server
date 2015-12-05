//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-02-22 15:08:37.
// File name: status_check_session_handler.h
//
// Description:
// Define class StatusCheckSessionHandler.
//

#ifndef __GATEWAY__SERVER__STATUS__CHECK__SESSION__HANDLER__H
#define __GATEWAY__SERVER__STATUS__CHECK__SESSION__HANDLER__H

#include "core/timer_handler.h"
#include "core/base/types.h"
#include "gateway_server/server/status_handler.h"

namespace gateway {

namespace server {

class GatewayUser;

class StatusCheckSessionHandler : public StatusHandler, public core::TimerHandler {
  enum { TIMER_CHECK_TIMEOUT = 0, TIMER_DELAY_CHECK, };
  enum { CHECK_TIMEOUT_SECS = 10, DELAY_CHECK_SECS = 5, };

 public:
  StatusCheckSessionHandler();
  virtual ~StatusCheckSessionHandler();

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
  void OnMessageLoginRequest(const char *message, size_t size);

  void OnCheckSessionFinish(const char *message, size_t size);
  void OnActorOnlineFinish(const char *message, size_t size);

  core::uint64 sequence_;
};

}  // namespace server

}  // namespace gateway

#endif  // __GATEWAY__SERVER__STATUS__CHECK__SESSION__HANDLER__H

