//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-02-22 15:01:04.
// File name: status_running_handler.h
//
// Description:
// Define class StatusRunningHandler.
//

#ifndef __GATEWAY__SERVER__STATUS__RUNNING__HANDLER__H
#define __GATEWAY__SERVER__STATUS__RUNNING__HANDLER__H

#include "core/base/types.h"
#include "gateway_server/server/status_handler.h"
#include "gateway_server/protocol/gateway_protocol_types.h"

namespace gateway {

namespace server {

class StatusRunningHandler : public StatusHandler {
 public:
  StatusRunningHandler();
  virtual ~StatusRunningHandler();

  virtual bool Initialize(GatewayUser *user);
  virtual void Finalize();

  virtual UserStatus::Type GetStatus();

  virtual void Start();
  virtual void End();

  virtual void GotoNextStatus();

  virtual void OnMessage(const char *message, size_t size);

  virtual void OnDisconnect();

 private:
  void OnMessageLogoutRequest(const char *message, size_t size);
  void OnMessageForward(protocol::Message &message);
  void OnMessageForwardSession(protocol::Message &message);
};

}  // namespace server

}  // namespace gateway

#endif  // __GATEWAY__SERVER__STATUS__RUNNING__HANDLER__H

