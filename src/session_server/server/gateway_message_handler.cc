//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-02-25 15:38:00.
// File name: gateway_message_handler.cc
//
// Description:
// Define class GatewayMessageHandler.
//

#include "session_server/server/gateway_message_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "database_server/client/database_client.h"
#include "database_server/protocol/storage_actor_login_types.h"
#include "session_server/protocol/game_actor_protocol_types.h"
#include "session_server/server/brief_actor.h"
#include "session_server/server/session_actor.h"
#include "session_server/server/session_actor_manager.h"
#include "session_server/server/session_actor_pool.h"
#include "session_server/server/session_terminal.h"

namespace session {
  
namespace server {

GatewayMessageHandler::GatewayMessageHandler() {}
GatewayMessageHandler::~GatewayMessageHandler() {}

bool GatewayMessageHandler::Initialize() {
  this->SetMessageHandler(protocol::GatewayMessageType::MESSAGE_USER_LOGIN_REQUEST,
      boost::bind(&GatewayMessageHandler::OnMessageUserLogin, this, _1, _2, _3));
  this->SetMessageHandler(protocol::GatewayMessageType::MESSAGE_USER_LOGOUT,
      boost::bind(&GatewayMessageHandler::OnMessageUserLogout, this, _1, _2, _3));
  this->SetMessageHandler(protocol::GatewayMessageType::MESSAGE_KICKOUT_USER,
      boost::bind(&GatewayMessageHandler::OnMessageKickoutUser, this, _1, _2, _3));

  return true;
} 

void GatewayMessageHandler::Finalize() {
  this->ResetMessageHandler(protocol::GatewayMessageType::MESSAGE_USER_LOGIN_REQUEST);
  this->ResetMessageHandler(protocol::GatewayMessageType::MESSAGE_USER_LOGOUT);
  this->ResetMessageHandler(protocol::GatewayMessageType::MESSAGE_KICKOUT_USER);
}

void GatewayMessageHandler::OnMessage(SessionTerminal *terminal,
    const char *message, size_t size) {
  if(terminal == NULL) {
    global::LogError("%s:%d (%s) terminal is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  global::ThriftPacket *thrift = terminal->GetThrift();
  if(thrift == NULL) {
    global::LogError("%s:%d (%s) thrift is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  protocol::MessageGatewayServer message2;
  if(thrift->Deserialize(message2, message, size) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  if(message2.type_ < protocol::GatewayMessageType::MESSAGE_MIN ||
        message2.type_ >= protocol::GatewayMessageType::MESSAGE_MAX ||
      this->handler_[message2.type_].empty() == true) {
    LOG_ERROR("Error message type[%u]", message2.type_);
    return ;
  }

  this->handler_[message2.type_](terminal, message2.message_.data(),
      message2.message_.size());
}

void GatewayMessageHandler::OnMessageUserLogin(SessionTerminal *terminal,
    const char *message, size_t size) {
  LOG_DEBUG("GatewayMessageHandler::OnMessageUserLogin");
  if(terminal == NULL) {
    global::LogError("%s:%d (%s) terminal is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  global::ThriftPacket *thrift = terminal->GetThrift();
  if(thrift == NULL) {
    global::LogError("%s:%d (%s) thrift is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // Deserialize message.
  protocol::GateMessageUserLoginRequest request;
  if(thrift->Deserialize(request, message, size) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 检测是否已登陆.
  if(SessionActorManager::GetInstance()->GetLoginActor(request.actor_id_) ||
      SessionActorManager::GetInstance()->GetActor(request.actor_id_)) {
    global::LogError("%s:%d (%s) Actor[%lld] has logined.",
        __FILE__, __LINE__, __FUNCTION__, request.actor_id_);
    // 踢掉玩家
    SessionActorManager::GetInstance()->KickActor(request.actor_id_);
    return ;
  }

  // 分配玩家对象
  SessionActor *actor = SessionActorPool::GetInstance()->Allocate();
  if(actor == NULL) {
    global::LogError("%s:%d (%s) Allocate actor failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  // 初始化玩家对象失败
  if(actor->Initialize() == false) {
    LOG_ERROR("Initialize actor failed.");
    SessionActorPool::GetInstance()->Deallocate(actor);
    return ;
  }
  actor->SetActorID(request.actor_id_);
  actor->SetGateway(terminal->GetSignature().id_);

  // 加入登陆队列
  if(SessionActorManager::GetInstance()->AddLoginActor(actor) == false) {
    global::LogError("%s:%d (%s) Add login actor list failed.",
        __FILE__, __LINE__, __FUNCTION__);
    actor->Finalize();
    SessionActorPool::GetInstance()->Deallocate(actor);
    return ;
  }

  // 玩家数据库请求登陆
  database::protocol::StorageActorLoginRequest data_request;
  data_request.__set_id_(actor->GetActorID());
  data_request.__set_gateway_(actor->GetGateway());
  size_t request_size = 0;
  const char *req = thrift->Serialize(data_request, request_size);
  if(req == NULL) {
    global::LogError("%s:%d (%s) 消息序列化失败", __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  // 请求
  if(database::client::DatabaseClientSingleton::GetInstance().SendRequest(
        database::protocol::StorageType::STORAGE_ACTOR_LOGIN,
        actor->GetActorID(), req, request_size) == false) {
    actor->Finalize();
    SessionActorPool::GetInstance()->Deallocate(actor);
    global::LogError("%s:%d (%s) 向数据库代理服务器请求失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
}

void GatewayMessageHandler::OnMessageUserLogout(SessionTerminal *terminal,
    const char *message, size_t size) {
  if(terminal == NULL) {
    global::LogError("%s:%d (%s) terminal is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  global::ThriftPacket *thrift = terminal->GetThrift();
  if(thrift == NULL) {
    global::LogError("%s:%d (%s) thrift is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 解析消息
  protocol::GateMessageUserLogout request;
  if(thrift->Deserialize(request, message, size) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  SessionActorManager::GetInstance()->KickActor(request.actor_id_);
}

void GatewayMessageHandler::OnMessageKickoutUser(SessionTerminal *terminal,
    const char *message, size_t size) {
  if(terminal == NULL) {
    global::LogError("%s:%d (%s) terminal is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  global::ThriftPacket *thrift = terminal->GetThrift();
  if(thrift == NULL) {
    global::LogError("%s:%d (%s) thrift is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 解析消息
  protocol::GateMessageKickoutUser request;
  if(thrift->Deserialize(request, message, size) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 找到玩家对象
  SessionActor *actor = SessionActorManager::GetInstance()->RemoveActor(
      request.actor_id_);
  if(actor == NULL) {
    // 如果踢出类型为重复登陆，则需要把中心数据库的登陆状态清除
    if(request.type_ == global::protocol::KickoutActorType::RELOGIN) {
      SessionServerSingleton::GetInstance().ActorLogoutCenter(request.actor_id_);
    }
    global::LogError("%s:%d (%s) Get actor is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 得到网关服务器在会话上的终端
  SessionTerminal *actor_gateway_terminal = actor->GetGatewayServer();
  if(actor_gateway_terminal == NULL) {
    global::LogError("%s:%d (%s) terminal is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 发送到目标网关
  actor_gateway_terminal->SendGatewayMessage(request,
      protocol::GatewayMessageType::MESSAGE_KICKOUT_USER);

  global::LogDebug("[踢出玩家流程] 网关请求踢出玩家(%llu, %s)， 原因(%d)",
      actor->GetActorID(), actor->GetName().c_str(), request.type_);
}

}  // namespace server

}  // namespace session

