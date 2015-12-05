//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-02-22 17:11:42.
// File name: game_client.cc
//
// Description:
// Define class GameClient.
//

#include "game_server/client/game_client.h"

#include "game_server/client/game_session.h"
#include "global/logging.h"

namespace game {

namespace client {

GameClient GameClientSingleton::instance_;

GameClient::GameClient() {}
GameClient::~GameClient() {}

bool GameClient::Initialize(const global::protocol::ServerSignature &signature) {
  this->signature_ = signature;
  return true;
}

void GameClient::Finalize() {
  GameSessionMap::iterator iterator = this->sessions_.begin();
  for(; iterator != this->sessions_.end(); ++iterator) {
    iterator->second->Finalize();
  }
  this->sessions_.clear();
}

void GameClient::Start() {}

void GameClient::Stop() {
  GameSessionMap::iterator iterator = this->sessions_.begin();
  for(; iterator != this->sessions_.end(); ++iterator) {
    iterator->second->Stop();
  }
}

void GameClient::OnGameServerJoin(core::EventLoop *network_loop,
    core::EventLoop *app_loop, const global::protocol::ServerSignature &signature) {
  if(signature.type_ != global::protocol::ServerType::GAME_SERVER) {
    global::LogError("%s:%d (%s) Error server type joined.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  // Allocate session.
  GameSessionPtr session(new (std::nothrow) GameSession());
  if(session.get() == NULL) {
    global::LogError("%s:%d (%s) Allocate game session failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  // Initialize sesssion.
  if(session->Initialize(core::InetAddress(signature.host_, signature.port_),
        "GameClient", network_loop, app_loop, signature, &this->thrift_) == false) {
    global::LogError("%s:%d (%s) Initialize game session failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  // Add session list.
  if(this->AddSession(session) == false) {
    global::LogError("%s:%d (%s) Add game session failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  session->Start();

  global::LogInfo("Start game session client[%s:%d]",
      signature.host_.c_str(), signature.port_);
}

void GameClient::OnGameServerDisjoin(const global::protocol::ServerSignature &signature) {
  if(signature.type_ != global::protocol::ServerType::GAME_SERVER) {
    global::LogError("%s:%d (%s) Error server type disjoined.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  this->StopSession(signature.id_);
  global::LogInfo("Game session client[%s:%d] stopped.",
      signature.host_.c_str(), signature.port_);
}

bool GameClient::AddSession(const GameSessionPtr &session) {
  if(session.get() == NULL) {
    global::LogError("%s:%d (%s) Argument session is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  GameSessionMap::iterator iterator = this->sessions_.find(
      session->GetServerSignature().id_);
  if(iterator != this->sessions_.end()) {
    return false;
  }
  this->sessions_.insert(std::make_pair(session->GetServerSignature().id_, session));
  return true;
}

bool GameClient::StopSession(core::uint32 server_id) {
  GameSessionMap::iterator iterator = this->sessions_.find(server_id);
  if(iterator == this->sessions_.end()) {
    return false;
  }
  iterator->second->Stop();
  return true;
}

bool GameClient::RemoveSession(core::uint32 server_id) {
  GameSessionMap::iterator iterator = this->sessions_.find(server_id);
  if(iterator == this->sessions_.end()) {
    return false;
  }
  this->sessions_.erase(iterator);
  return true;
}

GameSessionPtr GameClient::GetSession() {
  GameSessionPtr session;
  if(this->sessions_.empty()) {
    return session;
  }
  GameSessionMap::iterator iterator = this->sessions_.begin();
  session = iterator->second;
  for(; iterator != this->sessions_.end(); ++iterator) {
    if(session->GetActorNumber() < iterator->second->GetActorNumber()) {
      session = iterator->second;
    }
  }
  return session;
}

GameSessionPtr GameClient::GetSession(core::uint32 server_id) {
  GameSessionPtr session;
  GameSessionMap::iterator iterator = this->sessions_.find(server_id);
  if(iterator != this->sessions_.end()) {
    session = iterator->second;
  }
  return session;
}

void GameClient::OnMessage(protocol::MessageType::type type,
    const GameSessionPtr &session, const char *message, size_t size) {
  if(type < protocol::MessageType::MESSAGE_MIN ||
      type >= protocol::MessageType::MESSAGE_MAX ||
      message_handlers_[type].empty() == true) {
    global::LogError("%s:%d (%s) Error message type[%d]",
        __FILE__, __LINE__, __FUNCTION__, type);
    return ;
  }
  this->message_handlers_[type](session, message, size);
}


}  // namespace client

}  // namespace game

