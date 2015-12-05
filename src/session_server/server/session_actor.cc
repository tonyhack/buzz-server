//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-02-25 16:09:57.
// File name: session_actor.cc
//
// Description:
// Define class SessionActor.
//

#include "session_server/server/session_actor.h"

#include <cstddef>

#include "coresh/communicator.h"
#include "global/server_configure.h"
#include "session_server/protocol/game_actor_protocol_types.h"
#include "session_server/protocol/session_gateway_protocol_types.h"
#include "session_server/server/session_server.h"
#include "session_server/server/session_actor_manager.h"
#include "session_server/server/session_actor_pool.h"
#include "session_server/server/event/session_actor_event_types.h"
#include "session_server/server/event/session_event_types.h"

namespace session {

namespace server {

SessionActor::SessionActor()
  : brief_actor_(NULL), actor_id_(0), scene_(0), gateway_server_(0), game_server_(0),
    game_load_finish_(false), session_load_finish_(false) {}
SessionActor::~SessionActor() {}

bool SessionActor::Initialize() {
  this->actor_id_ = 0;
  this->scene_ = 0;

  this->game_server_ = 0;
  this->gateway_server_ = 0;

  if(this->load_progress_.Initialize() == false) {
    LOG_ERROR("Initialize load progress failed.");
    return false;
  }

  this->game_load_finish_ = false;
  this->session_load_finish_ = false;

  this->GetLoadProgress().LoadExtensionFinish(ExtensionType::GM);
  this->GetLoadProgress().LoadExtensionFinish(ExtensionType::SCENE);
  this->GetLoadProgress().LoadExtensionFinish(ExtensionType::CHAT);

  // 注册一个定时器，用于定时保存数据
  SessionServerSingleton::GetInstance().AddTimer(TIMER_ID_SAVE,
      global::ServerConfigureSingleton::GetInstance().GetAutoSave().actor_ * 1000,
      -1, this, "SessionActor::Initialize");

  return true;
}

void SessionActor::Finalize() {
  // 删除数据保存定时器
  SessionServerSingleton::GetInstance().RemoveTimer(TIMER_ID_SAVE, this);

  this->game_load_finish_ = false;
  this->session_load_finish_ = false;
  this->load_progress_.Finalize();
  this->gateway_server_ = 0;
  this->game_server_ = 0;
  this->scene_ = 0;
  this->actor_id_ = 0;
  this->brief_actor_ = NULL;
}

void SessionActor::SendMessage(gateway::protocol::MessageType::type type,
    const char *message, size_t size) {
  SessionTerminal *terminal = this->GetGatewayServer();
  if(terminal == NULL) {
    global::LogError("%s:%d (%s) Get terminal failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  protocol::MessageUserMessageForward forward;
  forward.type_ = type;
  forward.message_.assign(message, size);
  forward.actor_ = this->actor_id_;

  terminal->SendMessage(forward,
      protocol::MessageType::MESSAGE_USER_MESSAGE_FORWARD);
}

void SessionActor::OnGameLoadFinish() {
  this->game_load_finish_ = true;
  if(this->session_load_finish_ == true) {
    this->OnLoadFinish();
  }
}

void SessionActor::OnSessionLoadFinish() {
  this->session_load_finish_ = true;
  if(this->game_load_finish_ == true) {
    this->OnLoadFinish();
  }
}

void SessionActor::RequireLogout() {
  this->Save();
  this->Offline();
}

void SessionActor::OnLoadFinish() {
  SessionTerminal *gateway_terminal = this->GetGatewayServer();
  if(gateway_terminal) {
    // 发送登陆成功到网关
    protocol::GateMessageUserLoginResponse gate_response;
    gate_response.__set_actor_id_(this->GetActorID());
    gate_response.__set_game_server_(this->GetGame());
    gate_response.__set_logined_(true);
    gateway_terminal->SendGatewayMessage(gate_response,
        protocol::GatewayMessageType::MESSAGE_USER_LOGIN_RESPONSE);
    LOG_INFO("SessionActor 登陆完成");
  }
}

void SessionActor::Kickout() {
  SessionTerminal *gateway_terminal = this->GetGatewayServer();
  if(gateway_terminal) {
    protocol::GateMessageKickoutUser message;
    message.__set_actor_id_(this->GetActorID());
    message.__set_type_(global::protocol::KickoutActorType::UNKNOWN);
    gateway_terminal->SendGatewayMessage(message,
        protocol::GatewayMessageType::MESSAGE_KICKOUT_USER);
  }
}

void SessionActor::OnTimer(core::uint32 id) {
  if(id == TIMER_ID_SAVE) {
    this->Save();
  }
}

void SessionActor::Save() {
  // 发送保存事件
  event::EventActorSave event;
  event.__set_id_(this->GetActorID());
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_ACTOR_SAVE, this->GetActorID(), 
      event::ChannelType::CHANNEL_ACTOR, &event,
      sizeof(event::EventActorSave));
}

void SessionActor::Offline() {
  LOG_INFO("[玩家退出流程] 玩家(%llu, %s) 退出会话服务器",
    this->GetActorID(), this->GetName().c_str());
  
  // 发送登出事件
  event::EventActorLogout event;
  event.__set_id_(this->GetActorID());
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_ACTOR_LOGOUT, this->GetActorID(),
      event::ChannelType::CHANNEL_ACTOR, &event,
      sizeof(event::EventActorLogout));
}

}  // namespace server

}  // namespace session

