//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-02-25 16:09:19.
// File name: session_actor.h
//
// Description:
// Define class SessionActor.
//

#ifndef __SESSION__SERVER__SESSION__ACTOR__H
#define __SESSION__SERVER__SESSION__ACTOR__H

#include <cstdio>
#include <string>

#include "core/timer_handler.h"
#include "core/base/types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "global/game_misc.h"
#include "session_server/server/load_progress.h"
#include "session_server/server/brief_actor.h"
#include "session_server/server/session_server.h"
#include "session_server/server/session_terminal.h"

namespace session {

namespace server {

class SessionActor : public core::TimerHandler {
  enum { TIMER_ID_SAVE = 0 };

 public:
  SessionActor();
  ~SessionActor();

  bool Initialize();
  void Finalize();

  inline core::uint64 GetActorID() const {
    return this->actor_id_;
  }
  inline void SetActorID(core::uint64 id) {
    this->actor_id_ = id;
  }
  inline const std::string GetStringID() const {
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%lu", this->actor_id_);
    return std::string(buffer);
  }
  inline BriefActor *GetBriefActor() {
    return this->brief_actor_;
  }
  inline void SetBriefActor(BriefActor *brief_actor) {
    this->brief_actor_ = brief_actor;
  }

  inline const std::string &GetName() const {
    return this->brief_actor_->GetName();
  }
  inline core::int32 GetLevel() const {
    return this->brief_actor_->GetLevel();
  }
  inline entity::VocationType::type GetVocation() const {
    return this->brief_actor_->GetVocation();
  }
  inline entity::GenderType::type GetGender() const {
    return this->brief_actor_->GetGender();
  }
  inline bool CheckFunctionalityState(entity::FunctionalityType::type type) const {
    return this->brief_actor_->CheckFunctionalityState(type);
  }
  inline time_t GetLastLogoutTime() const {
    return this->brief_actor_->GetLastLogoutTime();
  }

  inline core::uint64 GetScene() const {
    return this->scene_;
  }
  inline void SetScene(core::uint64 scene) {
    this->scene_ = scene;
  }
  inline core::uint16 GetMap() const {
    return global::GameMisc::GetSceneMapID(this->GetScene());
  }

  inline core::uint32 GetGateway() const {
    return this->gateway_server_;
  }
  inline void SetGateway(core::uint32 id) {
    this->gateway_server_ = id;
  }

  inline core::uint32 GetGame() const {
    return this->game_server_;
  }
  inline void SetGameServer(core::uint32 id) {
    this->game_server_ = id;
  }

  inline SessionTerminal *GetGatewayServer() {
    return SessionServerSingleton::GetInstance().GetTerminal(
        global::protocol::ServerType::GATEWAY_SERVER, this->gateway_server_);
  }
  inline SessionTerminal *GetGameServer() {
    return SessionServerSingleton::GetInstance().GetTerminal(
        global::protocol::ServerType::GAME_SERVER, this->game_server_);
  }

  inline LoadProgress &GetLoadProgress() {
    return this->load_progress_;
  }

  void SendMessage(gateway::protocol::MessageType::type type,
      const char *message, size_t size);

  template <typename T>
  void SendMessage(const T &message, gateway::protocol::MessageType::type type);

  template <typename T>
  void SendGameMessage(const T &message, session::protocol::GameMessageType::type type);

  void RequireLogout();

  void OnGameLoadFinish();
  void OnSessionLoadFinish();

  // 踢出玩家
  void Kickout();

  virtual void OnTimer(core::uint32 id);

 private:
  void OnLoadFinish();

  void Save();
  void Offline();

  BriefActor *brief_actor_;
  core::uint64 actor_id_;
  core::uint64 scene_;

  core::uint32 gateway_server_;
  core::uint32 game_server_;

  // 加载进度
  LoadProgress load_progress_;

  // Game/Session 服务器加载完成
  bool game_load_finish_;
  bool session_load_finish_;
};

template <typename T>
void SessionActor::SendMessage(const T &message,
    gateway::protocol::MessageType::type type) {
  global::ThriftPacket *thrift =
    SessionServerSingleton::GetInstance().GetThrift();
  if(thrift == NULL) {
    global::LogError("%s:%d (%s) Get thrift packet is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  size_t size = 0;
  const char *message1 = thrift->Serialize(message, size);
  if(message1 == NULL) {
    global::LogError("%s:%d (%s) Serialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  this->SendMessage(type, message1, size);
}

template <typename T>
void SessionActor::SendGameMessage(const T &message,
    session::protocol::GameMessageType::type type) {
  SessionTerminal *terminal = this->GetGameServer();
  if (NULL == terminal) {
    LOG_ERROR("Get terminal failed.");
    return;
  }
  terminal->SendGameMessage(message, type);
}

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__SESSION__ACTOR__H

