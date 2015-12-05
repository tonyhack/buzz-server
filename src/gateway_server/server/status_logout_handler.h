//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-02-22 14:58:53.
// File name: status_logout_handler.h
//
// Description:
// Define class StatusLogoutHandler.
//

#ifndef __GATEWAY__SERVER__STATUS__LOGOUT__HANDLER__H
#define __GATEWAY__SERVER__STATUS__LOGOUT__HANDLER__H

#include "core/base/types.h"
#include "gateway_server/server/status_handler.h"

namespace gateway {

namespace server {

class StatusLogoutHandler : public StatusHandler {
 public:
  StatusLogoutHandler();
  virtual ~StatusLogoutHandler();

  virtual bool Initialize(GatewayUser *user);
  virtual void Finalize();

  virtual UserStatus::Type GetStatus();

  virtual void Start();
  virtual void End();

  virtual void GotoNextStatus();

  virtual void OnMessage(const char *message, size_t size);
};

}  // namespace server

}  // namespace gateway

#endif  // __GATEWAY__SERVER__STATUS__LOGOUT__HANDLER__H

