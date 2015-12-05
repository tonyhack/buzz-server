//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-02-22 15:03:59.
// File name: status_verify_question_handler.h
//
// Description:
// Define class StatusVerifyQuestionHandler.
//

#ifndef __GATEWAY__SERVER__STATUS__VERIFY__QUESTION__HANDLER__H
#define __GATEWAY__SERVER__STATUS__VERIFY__QUESTION__HANDLER__H

#include "core/timer_handler.h"
#include "core/base/types.h"
#include "gateway_server/server/status_handler.h"

namespace gateway {

namespace server {

class StatusVerifyQuestionHandler : public StatusHandler, public core::TimerHandler {
  enum { TIMER_VERIFY_QUESTION_TIMEOUT = 0, };
  enum { VERIFY_QUESTION_TIMEOUT_SECS = 10, };

 public:
  StatusVerifyQuestionHandler();
  virtual ~StatusVerifyQuestionHandler();

  virtual bool Initialize(GatewayUser *user);
  virtual void Finalize();

  virtual UserStatus::Type GetStatus();

  virtual void Start();
  virtual void End();

  virtual void GotoNextStatus();

  virtual void OnMessage(const char *message, size_t size);

  virtual void OnTimer(core::uint32 id);

 private:
  void SendQuestion();

  void OnMessageQuestionRequest(const char *message, size_t size);

  // Random verify variable.
  core::uint32 random_code_;
  char random_key_[32];
};

}  // namespace server

}  // namespace gateway

#endif  // __GATEWAY__SERVER__STATUS__VERIFY__QUESTION__HANDLER__H

