//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-02-05 14:13:15.
// File name: game_server.h
//
// Description:
// Define class GameServer.
//

#ifndef __GAME__SERVER__GAME__SERVER__H
#define __GAME__SERVER__GAME__SERVER__H

#include <map>
#include <set>

#include "core/async_logging_singleton.h"
#include "core/inet_address.h"
#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/application_service.h"
#include "game_server/protocol/game_protocol_types.h"
#include "game_server/server/day_pass.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/game_terminal.h"
#include "game_server/server/session_message_handler.h"
#include "game_server/server/storage_response.h"
#include "game_server/server/request/game_scene_request_types.h"
#include "game_server/server/request/game_request_types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "global/logging.h"
#include "global/server_type_types.h"
#include "global/thrift_packet.h"
#include "global/word_filter.h"
#include "schedule_server/client/schedule_client.h"
#include "session_server/client/session_client.h"

namespace game {

namespace server {

class GameServer : public core::Noncopyable {
  typedef std::pair<global::protocol::ServerType::type, core::uint32> TerminalKey;
  typedef std::map<TerminalKey, GameTerminal *> TerminalMap;
  typedef std::set<GameTerminal *> TerminalSet;

 public:
  GameServer(const core::InetAddress &address, const std::string &name);
  ~GameServer();

  bool Initialize(const global::protocol::ServerSignature &signature);
  void Finalize();

  void Start();
  void Stop();

  bool OnTerminalLogin(GameTerminal *terminal);
  bool OnTerminalLogout(GameTerminal *terminal);

  void OnDisconnect(GameTerminal *terminal);

  GameTerminal *GetGatewayTerminal(core::uint id);

  inline void AddTimer(core::uint32 id, core::uint32 msecs, int call_times,
      core::TimerHandler *handler, const std::string &debug) {
    this->service_.AddTimer(id, msecs, call_times, handler, debug);
  }
  inline void RemoveTimer(core::uint32 id, core::TimerHandler *handler) {
    this->service_.RemoveTimer(id, handler);
  }

  // Raw broadcast.
  template <typename T>
  void BroadcastGateway(const T &message, protocol::MessageType::type type);

  // To gateway server.
  template <typename T>
  void BroadcastMessage(const T &message, gateway::protocol::MessageType::type type);
  template <typename T>
  void BroadcastSceneMessage(const T &message, gateway::protocol::MessageType::type type,
      core::uint64 scene);
  template <typename T>
  void MulticastMessage(const T &message, gateway::protocol::MessageType::type type,
      core::uint64 actors[], size_t number);

  inline const global::protocol::ServerSignature &GetSignature() {
    return this->signature_;
  }
  inline void SetSignature(const global::protocol::ServerSignature &signature) {
    this->signature_ = signature;
  }

  inline global::ThriftPacket *GetThrift() {
    return &this->open_thrift_;
  }

  inline global::WordFilter *GetWordFilter() {
    return &this->word_filter_;
  }

  inline session::client::SessionClientPtr &GetSessionClient() {
    return this->session_client_;
  }

  inline SessionMessageHandler *GetSessionMessageHandler() {
    return &this->session_message_handler_;
  }

  // Get time tick.
  inline const core::TimeTick &GetTimeTick() const {
    return this->service_.GetTimeTick();
  }

 private:
  void OnConnection(const coresh::ConnectionPtr &connection);

  // join/disjoin schedule server.
  void OnServerJoinSchedule(const global::protocol::ServerSignature &signature);
  void OnServerDisjoinSchedule(const global::protocol::ServerSignature &signature);

  void OnDatabaseJoin(const global::protocol::ServerSignature &signature);
  void OnDatabaseDisjoin(const global::protocol::ServerSignature &signature);

  void OnSessionJoin(const global::protocol::ServerSignature &signature);
  void OnSessionDisjoin(const global::protocol::ServerSignature &signature);

  void OnGatewayJoin(const global::protocol::ServerSignature &signature);
  void OnGatewayDisjoin(const global::protocol::ServerSignature &signature);

  void OnLoginSession();

  global::protocol::ServerSignature signature_;
  coresh::ApplicationService service_;

  schedule::client::ScheduleClientPtr schedule_client_;
  session::client::SessionClientPtr session_client_;

  TerminalMap terminals_;
  TerminalSet connections_;

  SessionMessageHandler session_message_handler_;
  StorageResponse storage_response_handler_;

  global::ThriftPacket thrift_;
  global::ThriftPacket open_thrift_;
  global::WordFilter word_filter_;

  DayPass day_pass_;

  static const size_t kMaxMulticastSize_ = 500;
};

template <typename T>
void GameServer::BroadcastGateway(const T &message,
    protocol::MessageType::type type) {
  TerminalMap::iterator iterator = this->terminals_.begin();
  for(; iterator != this->terminals_.end(); ++iterator) {
    if(iterator->first.first == global::protocol::ServerType::GATEWAY_SERVER) {
      iterator->second->SendMessage(message, type);
    }
  }
}

template <typename T>
void GameServer::MulticastMessage(const T &message,
    gateway::protocol::MessageType::type type, core::uint64 actors[], size_t number) {
  size_t size = 0;
  const char *message2 = this->thrift_.Serialize(message, size);
  if(message2 == NULL) {
    global::LogError("%s:%d (%s) 序列化消息失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  protocol::MessageUserMessageMulticast multicast;
  multicast.__set_type_(type);
  multicast.message_.assign(message2, size);
  size_t pos = 0;
  for(; pos < number; ++pos) {
    multicast.actors_.push_back(actors[pos]);
    if(multicast.actors_.size() >= kMaxMulticastSize_) {
      this->BroadcastGateway(multicast,
          protocol::MessageType::MESSAGE_USER_MESSAGE_MULTICAST);
      multicast.actors_.clear();
    }
  }
  if(multicast.actors_.empty() == false) {
    this->BroadcastGateway(multicast,
        protocol::MessageType::MESSAGE_USER_MESSAGE_MULTICAST);
  }
}

template <typename T>
void GameServer::BroadcastMessage(const T &message,
    gateway::protocol::MessageType::type type) {
  size_t size = 0;
  const char *message2 = this->thrift_.Serialize(message, size);
  if(message2 == NULL) {
    global::LogError("%s:%d (%s) 序列化消息失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  protocol::MessageUserMessageBroadcast broadcast;
  broadcast.__set_type_(type);
  broadcast.message_.assign(message2, size);
  this->BroadcastGateway(broadcast,
      protocol::MessageType::MESSAGE_USER_MESSAGE_BROADCAST);
}

template <typename T>
void GameServer::BroadcastSceneMessage(const T &message,
    gateway::protocol::MessageType::type type, core::uint64 scene) {
  size_t size = 0;
  const char *message2 = this->thrift_.Serialize(message, size);
  if(message2 == NULL) {
    global::LogError("%s:%d (%s) 序列化消息失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  request::RequestSceneActorBroadcast broadcast;
  broadcast.__set_type_(type);
  broadcast.__set_scene_(scene);
  broadcast.message_.assign(message2, size);
  if(ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_SCENE_ACTOR_BROADCAST,
        &broadcast, sizeof(broadcast)) == -1) {
    global::LogError("%s:%d (%s) 请求失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
}

class GameServerSingleton : public core::Noncopyable {
 public:
  GameServerSingleton() {}
  ~GameServerSingleton() {}

  static inline void SetInstance(GameServer *instance) {
    GameServerSingleton::instance_ = instance;
  }
  static inline GameServer &GetInstance() {
    return *GameServerSingleton::instance_;
  }

 private:
  static GameServer *instance_;
};

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__GAME__SERVER__H

