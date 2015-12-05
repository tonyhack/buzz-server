//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-02-22 14:57:54.
// File name: status_login_handler.cc
//
// Description:
// Define class StatusLoginHandler.
//

#include "gateway_server/server/status_login_handler.h"

#include "database_server/client/database_client.h"
#include "database_server/protocol/storage_check_session_types.h"
#include "game_server/protocol/game_protocol_types.h"
#include "gateway_server/server/gateway_server.h"
#include "gateway_server/server/gateway_user.h"
#include "gateway_server/server/session_channel.h"
#include "session_server/protocol/session_gateway_protocol_types.h"

namespace gateway {

namespace server {

static core::uint8 kExpectedSessionStatus = 2;
static core::uint32 kSessionEffectiveTime = 60;

StatusLoginHandler::StatusLoginHandler() {}
StatusLoginHandler::~StatusLoginHandler() {}

bool StatusLoginHandler::Initialize(GatewayUser *user) {
  return StatusHandler::Initialize(user);
}

void StatusLoginHandler::Finalize() {
  StatusHandler::Finalize();
  GatewayServerSingleton::GetInstance().RemoveTimer(TIMER_LOGIN_TIMEOUT, this);
}

UserStatus::Type StatusLoginHandler::GetStatus() {
  return UserStatus::LOGIN_STATUS;
}

void StatusLoginHandler::Start() {
  GatewayServerSingleton::GetInstance().AddTimer(TIMER_LOGIN_TIMEOUT,
      LOGIN_TIMEOUT_SECS * 1000, 1, this, "StatusLoginHandler::Start");
  this->OnRequestUserLogin();
}

void StatusLoginHandler::End() {
  GatewayServerSingleton::GetInstance().RemoveTimer(TIMER_LOGIN_TIMEOUT, this);
}

void StatusLoginHandler::GotoNextStatus() {
  if(this->user_ == NULL) {
    global::LogError("%s:%d (%s) Gateway user pointer is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  this->user_->GotoStatus(UserStatus::RUNNING_STATUS);
}

void StatusLoginHandler::OnMessage(const char *message, size_t size) {
  global::ThriftPacket *thrift = GatewayServerSingleton::GetInstance().GetThrift();
  if(thrift == NULL) {
    global::LogError("%s:%d (%s) Get thrift packet failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  protocol::Message message2;
  if(thrift->Deserialize(message2, message, size) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  switch(message2.type_) {
    default:
      global::LogError("%s:%d (%s) Error message type[%d]",
          __FILE__, __LINE__, __FUNCTION__, message2.type_);
      break;
  }
}


void StatusLoginHandler::OnEvent(StatusEventType::type type,
    const char *event, size_t size) {
  switch(type) {
    case StatusEventType::EVENT_USER_LOGIN:
      this->OnUserLoginFinish(event, size);
      break;
    default:
      break;
  }
}

void StatusLoginHandler::OnTimer(core::uint32 id) {
  if(this->user_ == NULL) {
    global::LogError("%s:%d (%s) Gateway user pointer is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  if(id == TIMER_LOGIN_TIMEOUT) {
    this->SendLogoutSession();
    this->user_->GotoStatus(UserStatus::LOGOUT_STATUS);
    global::LogDebug("Gateway user[%lld] login timeout!",
        this->user_->GetActorID().GetID());
  }
}

void StatusLoginHandler::OnDisconnect() {
  this->SendLogoutSession();
}

void StatusLoginHandler::SendLogoutSession() {
  if(this->user_ == NULL) {
    global::LogError("%s:%d (%s) Gateway user pointer is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  LOG_INFO("[登陆超时] 玩家(%u)登陆超时 发送登出消息到会话",
      this->user_->GetActorID().GetID());

  session::protocol::GateMessageUserLogout message;
  message.__set_actor_id_(this->user_->GetActorID().GetID());
  SessionChannel::SendMessage(message,
      session::protocol::GatewayMessageType::MESSAGE_USER_LOGOUT);
}

void StatusLoginHandler::OnUserLoginFinish(const char *message, size_t size) {
  if(sizeof(StatusEventUserLogin) != size) {
    global::LogError("%s:%d (%s) Argument size[%d] error",
        __FILE__, __LINE__, __FUNCTION__, size);
    return ;
  }
  if(this->user_ == NULL) {
    global::LogError("%s:%d (%s) Gateway user pointer is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  StatusEventUserLogin *event = (StatusEventUserLogin *)message;
  // 发送给玩家登陆结果
  protocol::MessageLoginResponse response;
  response.logined_ = event->result_;
  this->user_->SendMessage(response, protocol::MessageType::MESSAGE_LOGIN_RESPONSE);
  // 成功进入下一状态，失败进入退出状态
  if(event->result_ == true) {
    this->GotoNextStatus();
  } else {
    this->SendLogoutSession();
    this->user_->GotoStatus(UserStatus::LOGOUT_STATUS);
  }
}

void StatusLoginHandler::OnRequestUserLogin() {
  if(this->user_ == NULL) {
    global::LogError("%s:%d (%s) Gateway user pointer is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  // Login to session server.
  session::protocol::GateMessageUserLoginRequest request;
  request.__set_actor_id_(this->user_->GetActorID().serialize_);
  session::client::SessionClientPtr session =
    GatewayServerSingleton::GetInstance().GetSessionClient();
  if(session.get() == NULL) {
    global::LogError("%s:%d (%s) Get session client failed.",
        __FILE__, __LINE__, __FUNCTION__);
    this->user_->GotoStatus(UserStatus::LOGOUT_STATUS);
    return ;
  }
  SessionChannel::SendMessage(request,
      session::protocol::GatewayMessageType::MESSAGE_USER_LOGIN_REQUEST);
}

}  // namespace server

}  // namespace gateway

