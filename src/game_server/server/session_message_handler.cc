//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-02-25 18:00:32.
// File name: session_message_handler.cc
//
// Description:
// Define class SessionMessageHandler.
//

#include "game_server/server/session_message_handler.h"

#include <boost/bind.hpp>

#include "database_server/client/database_client.h"
#include "database_server/protocol/storage_actor_login_types.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/game_actor_pool.h"
#include "game_server/server/game_server.h"
#include "game_server/server/session_channel.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "global/channel.h"
#include "global/game_misc.h"
#include "global/logging.h"
#include "session_server/client/session_client.h"
#include "session_server/protocol/game_actor_protocol_types.h"

namespace game {

namespace server {

SessionMessageHandler::SessionMessageHandler() : thrift_(NULL) {}
SessionMessageHandler::~SessionMessageHandler() {}

bool SessionMessageHandler::Initialize(global::ThriftPacket *thrift) {
  if(thrift == NULL) {
    global::LogError("%s:%d (%s) Initialize session message handler failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false; 
  } 

  this->thrift_ = thrift;

  session::client::SessionClientPtr session =
    GameServerSingleton::GetInstance().GetSessionClient();
  if(session.get() == NULL) {
    global::LogError("%s:%d (%s) Get session client failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  session->SetMessageHandler(session::protocol::MessageType::MESSAGE_GAME_SERVER,
      boost::bind(&SessionMessageHandler::OnMessageGameServer, this, _1, _2));

  this->SetMessageHandler(session::protocol::GameMessageType::MESSAGE_ACTOR_LOGIN_REQUEST,
      boost::bind(&SessionMessageHandler::OnMessageUserLogin, this, _1, _2));
  this->SetMessageHandler(session::protocol::GameMessageType::MESSAGE_ACTOR_LOGOUT,
      boost::bind(&SessionMessageHandler::OnMessageUserLogout, this, _1, _2));

  return true;
}

void SessionMessageHandler::Finalize() {
  this->ResetMessageHandler(session::protocol::GameMessageType::MESSAGE_ACTOR_LOGIN_REQUEST);
  this->ResetMessageHandler(session::protocol::GameMessageType::MESSAGE_ACTOR_LOGOUT);

  session::client::SessionClientPtr session =
    GameServerSingleton::GetInstance().GetSessionClient();
  if(session.get() == NULL) {
    global::LogError("%s:%d (%s) Get session client failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  session->ResetMessageHandler(session::protocol::MessageType::MESSAGE_GAME_SERVER);
}

void SessionMessageHandler::OnMessageGameServer(const char *message, size_t size) {
  if(this->thrift_ == NULL) {
    global::LogError("%s:%d (%s) Thrift packet is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  session::protocol::MessageGameServer message2;
  if(this->thrift_->Deserialize(message2, message, size) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  if(message2.type_ < session::protocol::GameMessageType::MESSAGE_MIN ||
      message2.type_ >= session::protocol::GameMessageType::MESSAGE_MAX ||
      handler_[message2.type_].empty() == true) {
    global::LogError("%s:%d (%s) Error message type[%d]",
        __FILE__, __LINE__, __FUNCTION__, message2.type_);
    return ;
  }
  this->handler_[message2.type_](message2.message_.data(), message2.message_.size());
}

void SessionMessageHandler::OnMessageUserLogin(const char *message, size_t size) {
  if(this->thrift_ == NULL) {
    LOG_ERROR("thrift_ 指针为空");
    return ;
  }
  // 消息解析
  session::protocol::GameMessageActorLoginRequest message2;
  if(this->thrift_->Deserialize(message2, message, size) == false) {
    LOG_ERROR("消息解析失败，玩家登陆失败");
    return ;
  }
  // 检测管理器中是否有该玩家ID
  GameActor *check_actor = GameActorManager::GetInstance()->GetActor(message2.actor_id_);
  if (check_actor != NULL) {
    this->ResponseSession(message2.actor_id_, false);
    LOG_ERROR("角色(%lu) 重复登陆游戏服务器", message2.actor_id_);
    // 踢掉玩家
    GameActorManager::GetInstance()->KickActor(check_actor->GetActorID());
    return ;
  }

  // 如果是跳场景登陆，需要从数据库读出基本数据
  if(message2.jump_scene_ == true) {
    database::protocol::StorageActorLoginRequest data_request;
    data_request.__set_id_(message2.actor_id_);
    data_request.__set_gateway_(message2.gateway_);
    data_request.__set_jump_scene_(message2.scene_);
    data_request.__set_jump_x_(message2.x_);
    data_request.__set_jump_y_(message2.y_);
    if(database::client::DatabaseClientSingleton::GetInstance().SendRequest(
          data_request,database::protocol::StorageType::STORAGE_ACTOR_LOGIN,
          message2.actor_id_) == false) {
      LOG_ERROR("角色(%lu) 跨服跳场景时，请求登陆服务器失败");
      return ;
    }
    return ;
  }

  // 正常登陆流程
  if(message2.__isset.data_ == false) {
    LOG_ERROR("角色(%lu) 登陆失败");
    return ;
  }

  // 玩家数据解析
  database::protocol::StorageActorLoginResponse response;
  if(this->thrift_->Deserialize(response, message2.data_) == false) {
    this->ResponseSession(message2.actor_id_, false);
    LOG_ERROR("玩家数据解析失败，玩家(%lu)登陆游戏服务器失败", message2.actor_id_);
    return ;
  }

  // 分配玩家对象
  GameActor *actor = GameActorPool::GetInstance()->Allocate();
  if(actor == NULL) {
    this->ResponseSession(message2.actor_id_, false);
    GameActorPool::GetInstance()->Deallocate(actor);
    LOG_ERROR("分配玩家对象失败，玩家(%lu)登陆游戏服务器失败", message2.actor_id_);
    return ;
  }

  actor->SetActorID(message2.actor_id_);
  actor->SetGateway(message2.gateway_);

  // 初始化玩家对象
  if(actor->Initialize(response.field_) == false) {
    this->ResponseSession(message2.actor_id_, false);
    GameActorPool::GetInstance()->Deallocate(actor);
    LOG_ERROR("初始化玩家对象失败，玩家(%lu)登陆游戏服务器失败", message2.actor_id_);
    return ;
  }

  // 加入玩家管理器
  if(GameActorManager::GetInstance()->AddActor(actor) == false) {
    this->ResponseSession(message2.actor_id_, false);
    actor->Finalize();
    GameActorPool::GetInstance()->Deallocate(actor);
    LOG_ERROR("加入管理器失败，玩家(%lu)登陆游戏服务器失败", message2.actor_id_);
    return ;
  }

  if(message2.scene_ == 0) {
    LOG_ERROR("玩家(%lu) 登陆时 message.scene_ 为0", message2.actor_id_);
    return;
  }

  actor->SetScene(message2.scene_);
  actor->SetPosition(message2.x_, message2.y_);

  LOG_INFO("玩家(%lu, %s) 场景(%lu) 登陆游戏服务器成功，开始加载扩展数据",
      actor->GetActorID(), actor->GetName().c_str(), actor->GetScene());

  // 发送登陆事件
  event::EventActorLogin event;
  event.__set_id_(actor->GetID());
  event.__set_map_(response.field_.map_);
  event.__set_x_(response.field_.x_);
  event.__set_y_(response.field_.y_);

  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_ACTOR_LOGIN, actor->GetID(),
      event::ChannelType::CHANNEL_ACTOR, &event,
      sizeof(event::EventActorLogin));
}

void SessionMessageHandler::ResponseSession(core::uint64 actor, bool success) {
  session::protocol::GameMessageActorLoginResponse response_message;
  response_message.__set_actor_id_(actor);
  response_message.__set_logined_(success);
  response_message.__set_jump_scene_(false);
  SessionChannel::SendMessage(response_message,
      session::protocol::GameMessageType::MESSAGE_ACTOR_LOGIN_RESPONSE);
}

void SessionMessageHandler::OnMessageUserLogout(const char *message, size_t size) {
  if(this->thrift_ == NULL) {
    global::LogError("%s:%d (%s) Thrift packet is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  // 消息解析
  session::protocol::GameMessageActorLogout logout;
  if(this->thrift_->Deserialize(logout, message, size) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  // 从管理器中找到玩家对象
  GameActor *actor = GameActorManager::GetInstance()->GetActor(logout.actor_id_);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) Get game actor[%llu] failed.", 
        __FILE__, __LINE__, __FUNCTION__, logout.actor_id_);
    return ;
  }
  GameActorManager::GetInstance()->KickActor(actor->GetActorID());
}

}  // namespace server

}  // namespace game

