//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-31 16:33:09.
// File name: gateway_user.h
//
// Description:
// Define class GatewayUser.
//

#ifndef __GATEWAY__SERVER__GATEWAY__USER__H
#define __GATEWAY__SERVER__GATEWAY__USER__H

#include "coresh/connection.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/server/check_session_queue.h"
#include "gateway_server/server/gateway_server.h"
#include "gateway_server/server/status_handler.h"
#include "gateway_server/server/status_check_session_handler.h"
#include "gateway_server/server/status_login_handler.h"
#include "gateway_server/server/status_logout_handler.h"
#include "gateway_server/server/status_running_handler.h"
#include "gateway_server/server/status_verify_question_handler.h"
#include "global/actor_id.h"


namespace gateway {

namespace server {

class GatewayUser : public core::Noncopyable {
 public:
  GatewayUser();
  ~GatewayUser();

  bool Initialize(const coresh::ConnectionPtr &connection);
  void Finalize();

  template <typename T>
  void SendMessage(const T &message, protocol::MessageType::type type);

  void SendMessage(const protocol::Message &message);
  void SendMessage(const char *message, size_t size);
/*
  void FinishCheckSession(bool pass);
  void FinishUserLogin(bool pass);
*/
  void OnEvent(StatusEventType::type type, const char *event, size_t size);

  void GotoStatus(UserStatus::Type type);
  void Exit();

  inline bool GetLogined() const {
    return this->logined_;
  }
  inline void SetLogined(bool logined) {
    this->logined_ = logined;
  }

  inline UserStatus::Type GetUserStatus() const {
    if(this->status_) {
      return this->status_->GetStatus();
    } else {
      return UserStatus::MAX_STATUS;
    }
  }

  inline void SetActorID(const std::string &id) {
    this->id_.Deserialize(id.c_str());
  }
  inline const global::ActorID &GetActorID() const {
    return this->id_;
  }

  inline void SetGameServer(core::uint32 server) {
    this->game_server_ = server;
  }
  inline core::uint32 GetGameServer() const {
    return this->game_server_;
  }

 private:
  void OnDisconnect();
  void OnMessage(const char *message, size_t size);

  coresh::ConnectionPtr connection_;

  global::ActorID id_;

  core::uint32 game_server_;

  bool logined_;

  StatusHandler *status_;
  StatusCheckSessionHandler check_session_status_;
  StatusVerifyQuestionHandler verify_question_status_;
  StatusLoginHandler login_status_;
  StatusRunningHandler running_status_;
  StatusLogoutHandler logout_status_;
};

template <typename T>
void GatewayUser::SendMessage(const T &message,
    protocol::MessageType::type type) {
  size_t size = 0;
  const char *message1 =
    GatewayServerSingleton::GetInstance().GetThrift()->Serialize(message, size);
  if(message1) {
    protocol::Message message2;
    message2.__set_type_(type);
    message2.message_.assign(message1, size);
    this->SendMessage(message2);
  } else {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
  }
}

}  // namespace server

}  // namespace gateway

#endif  // __GATEWAY__SERVER__GATEWAY__USER__H

