//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-02-22 15:12:14.
// File name: status_check_session_handler.cc
//
// Description:
// Define class StatusCheckSessionHandler.
//

#include "gateway_server/server/status_check_session_handler.h"

#include "database_server/client/database_client.h"
#include "database_server/protocol/storage_actor_online_types.h"
#include "database_server/protocol/storage_check_session_types.h"
#include "gateway_server/server/gateway_server.h"
#include "gateway_server/server/gateway_user.h"

namespace gateway {

namespace server {

static core::uint8 kExpectedSessionStatus = 2;
static core::uint32 kSessionEffectiveTime = 99999;

StatusCheckSessionHandler::StatusCheckSessionHandler() : sequence_(0) {}
StatusCheckSessionHandler::~StatusCheckSessionHandler() {}

bool StatusCheckSessionHandler::Initialize(GatewayUser *user) {
  this->sequence_ = 0;
  return StatusHandler::Initialize(user);
}

void StatusCheckSessionHandler::Finalize() {
  StatusHandler::Finalize();
  GatewayServerSingleton::GetInstance().RemoveTimer(TIMER_CHECK_TIMEOUT, this);
  GatewayServerSingleton::GetInstance().RemoveTimer(TIMER_DELAY_CHECK, this);
}

UserStatus::Type StatusCheckSessionHandler::GetStatus() {
  return UserStatus::CHECK_SESSION_STATUS;
}

void StatusCheckSessionHandler::Start() {
  GatewayServerSingleton::GetInstance().AddTimer(TIMER_CHECK_TIMEOUT,
      CHECK_TIMEOUT_SECS * 1000, 1, this, "StatusCheckSessionHandler::Start");
}

void StatusCheckSessionHandler::End() {
  GatewayServerSingleton::GetInstance().RemoveTimer(TIMER_CHECK_TIMEOUT, this);
  GatewayServerSingleton::GetInstance().RemoveTimer(TIMER_DELAY_CHECK, this);
  if(this->sequence_ != 0) {
    CheckSessionQueue::GetInstance()->Remove(this->sequence_);
    this->sequence_ = 0;
  }
}

void StatusCheckSessionHandler::GotoNextStatus() {
  if(this->user_ == NULL) {
    global::LogError("%s:%d (%s) Gateway user pointer is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  this->user_->GotoStatus(UserStatus::LOGIN_STATUS);
}

void StatusCheckSessionHandler::OnMessage(const char *message, size_t size) {
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
    case protocol::MessageType::MESSAGE_LOGIN_REQUEST:
      this->OnMessageLoginRequest(message2.message_.data(), message2.message_.size());
      break;
    default:
      global::LogError("%s:%d (%s) Error message type[%d]",
          __FILE__, __LINE__, __FUNCTION__, message2.type_);
      break;
  }
}

void StatusCheckSessionHandler::OnEvent(StatusEventType::type type,
    const char *event, size_t size) {
  switch(type) {
    case StatusEventType::EVENT_CHECK_SESSION:
      this->OnCheckSessionFinish(event, size);
      break;
    case StatusEventType::EVENT_USER_ONLINE:
      this->OnActorOnlineFinish(event, size);
      break;
    default:
      break;
  }
}

void StatusCheckSessionHandler::OnTimer(core::uint32 id) {
  if(this->user_ == NULL) {
    global::LogError("%s:%d (%s) Gateway user pointer is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  global::ThriftPacket *thrift = GatewayServerSingleton::GetInstance().GetThrift();
  if(thrift == NULL) {
    global::LogError("%s:%d (%s) Thrift is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  if(id == TIMER_CHECK_TIMEOUT) {
    this->user_->GotoStatus(UserStatus::LOGOUT_STATUS);
    global::LogDebug("Gateway user[%lld] check session timeout!",
        this->user_->GetActorID().GetID());
  } else if(id == TIMER_DELAY_CHECK) {
    // 分配序列号，加入队列
    if(CheckSessionQueue::GetInstance()->Insert(this->user_,
          this->sequence_) == false) {
      global::LogError("%s:%d (%s) Send database request failed.",
          __FILE__, __LINE__, __FUNCTION__);
      this->user_->GotoStatus(UserStatus::LOGOUT_STATUS);
      return ;
    }
    // Check session.
    database::protocol::StorageActorOnlineRequest storage;
    storage.__set_id_(this->user_->GetActorID().GetID());
    storage.__set_sequence_(this->sequence_);
    size_t storage_size = 0;
    const char *storage_ptr = thrift->Serialize(storage, storage_size);
    if(storage_ptr == NULL) {
      global::LogError("%s:%d (%s) Serialize message failed.",
          __FILE__, __LINE__, __FUNCTION__);
      this->user_->GotoStatus(UserStatus::LOGOUT_STATUS);
      CheckSessionQueue::GetInstance()->Remove(this->sequence_);
      return ;
    }
    // 发送session检测到数据库代理
    if(database::client::DatabaseClientSingleton::GetInstance().SendRequest(
          database::protocol::StorageType::STORAGE_ACTOR_ONLINE,
          this->user_->GetActorID().GetID(), storage_ptr, storage_size) == false) {
      global::LogError("%s:%d (%s) Send database request failed.",
          __FILE__, __LINE__, __FUNCTION__);
      this->user_->GotoStatus(UserStatus::LOGOUT_STATUS);
      CheckSessionQueue::GetInstance()->Remove(this->sequence_);
      return ;
    }
  }
}

void StatusCheckSessionHandler::OnDisconnect() {
  if(this->sequence_) {
    CheckSessionQueue::GetInstance()->Remove(this->sequence_);
  }
}

void StatusCheckSessionHandler::OnMessageLoginRequest(const char *message, size_t size) {
  if(this->user_ == NULL) {
    global::LogError("%s:%d (%s) Gateway user pointer is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  global::ThriftPacket *thrift = GatewayServerSingleton::GetInstance().GetThrift();
  if(thrift == NULL) {
    global::LogError("%s:%d (%s) Thrift is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 消息解析
  protocol::MessageLoginRequest request;
  if(thrift->Deserialize(request, message, size) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  this->user_->SetActorID(request.actor_id_);

  global::LogDebug("[玩家登陆流程] 玩家(%s)发来登陆请求", request.actor_id_.c_str());

  // 分配序列号，加入检测队列
  if(CheckSessionQueue::GetInstance()->Insert(this->user_,
        this->sequence_) == false) {
    global::LogError("%s:%d (%s) Send database request failed.",
        __FILE__, __LINE__, __FUNCTION__);
    this->user_->GotoStatus(UserStatus::LOGOUT_STATUS);
    return ;
  }
  // Check session.
  database::protocol::StorageCheckSessionRequest storage;
  storage.__set_id_(this->user_->GetActorID().GetID());
  storage.__set_session_(request.session_key_);
  storage.__set_session_status_(kExpectedSessionStatus);
  storage.__set_effective_time_(kSessionEffectiveTime);
  storage.__set_sequence_(this->sequence_);
  size_t storage_size = 0;
  const char *storage_ptr = thrift->Serialize(storage, storage_size);
  if(storage_ptr == NULL) {
    global::LogError("%s:%d (%s) Serialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    this->user_->GotoStatus(UserStatus::LOGOUT_STATUS);
    CheckSessionQueue::GetInstance()->Remove(this->sequence_);
    return ;
  }
  // 发送session检测到数据库代理
  if(database::client::DatabaseClientSingleton::GetInstance().SendRequest(
        database::protocol::StorageType::STORAGE_CHECK_SESSION,
        this->user_->GetActorID().GetID(), storage_ptr, storage_size) == false) {
    global::LogError("%s:%d (%s) Send database request failed.",
        __FILE__, __LINE__, __FUNCTION__);
    this->user_->GotoStatus(UserStatus::LOGOUT_STATUS);
    CheckSessionQueue::GetInstance()->Remove(this->sequence_);
    return ;
  }
}

void StatusCheckSessionHandler::OnCheckSessionFinish(const char *message, size_t size) {
  this->sequence_ = 0;
  // 参数检测
  if(sizeof(StatusEventCheckSession) != size) {
    global::LogError("%s:%d (%s) Argument size[%d] error",
        __FILE__, __LINE__, __FUNCTION__, size);
    return ;
  }
  if(this->user_ == NULL) {
    global::LogError("%s:%d (%s) Gateway user pointer is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  StatusEventCheckSession *event = (StatusEventCheckSession *)message;
  if(event->result_ == false) {
    // 检测失败转到退出状态
    this->user_->GotoStatus(UserStatus::LOGOUT_STATUS);
    return ;
  }
  // 检测成功
  if(event->online_ == false) {
    // 玩家当前不在线
    if(GatewayServerSingleton::GetInstance().Login(this->user_) == false) {
      protocol::MessageLoginResponse response;
      response.logined_ = false;
      this->user_->SendMessage(response, protocol::MessageType::MESSAGE_LOGIN_RESPONSE);
      this->user_->GotoStatus(UserStatus::LOGOUT_STATUS);
      return ;
    }
    this->GotoNextStatus();
  } else {
    GatewayServerSingleton::GetInstance().KickOut(this->user_->GetActorID().GetID(),
        global::protocol::KickoutActorType::RELOGIN);
    // 玩家当前在线
    GatewayServerSingleton::GetInstance().AddTimer(TIMER_DELAY_CHECK,
          DELAY_CHECK_SECS * 1000, 1, this, "StatusCheckSessionHandler::OnCheckSessionFinish");
  }
}

void StatusCheckSessionHandler::OnActorOnlineFinish(const char *message, size_t size) {
  // 参数检测
  if(sizeof(StatusEventUserOnline) != size) {
    global::LogError("%s:%d (%s) Argument size[%d] error",
        __FILE__, __LINE__, __FUNCTION__, size);
    return ;
  }
  if(this->user_ == NULL) {
    global::LogError("%s:%d (%s) Gateway user pointer is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  StatusEventUserOnline *event = (StatusEventUserOnline *)message;
  // 上线失败
  if(event->online_ != 0) {
    this->user_->GotoStatus(UserStatus::LOGOUT_STATUS);
    return ;
  }
  // 之前在线的玩家已退出，则登入并切换到下一状态
  if(GatewayServerSingleton::GetInstance().Login(this->user_) == false) {
    protocol::MessageLoginResponse response;
    response.logined_ = false;
    this->user_->SendMessage(response, protocol::MessageType::MESSAGE_LOGIN_RESPONSE);
    this->user_->GotoStatus(UserStatus::LOGOUT_STATUS);
    return ;
  }
  this->GotoNextStatus();
}

}  // namespace server

}  // namespace gateway

