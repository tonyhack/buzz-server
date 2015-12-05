//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-02-04 09:50:47.
// File name: session_server.h
//
// Description:
// Define class SessionServer.
//

#ifndef __SESSION__SERVER__SESSION__SERVER__H
#define __SESSION__SERVER__SESSION__SERVER__H

#include <map>
#include <set>

#include "core/async_logging_singleton.h"
#include "core/inet_address.h"
#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/application_service.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "global/logging.h"
#include "global/server_type_types.h"
#include "global/thrift_packet.h"
#include "global/word_filter.h"
#include "schedule_server/client/schedule_client.h"
#include "session_server/protocol/session_protocol_types.h"
#include "session_server/protocol/session_game_protocol_types.h"
#include "session_server/server/day_pass.h"
#include "session_server/server/game_server_index.h"
#include "session_server/server/session_terminal.h"
#include "session_server/server/storage_response.h"

namespace session {

namespace server {

class SessionServer : public core::Noncopyable {
  typedef std::pair<global::protocol::ServerType::type, core::uint32> TerminalKey;
  typedef std::map<TerminalKey, SessionTerminal *> TerminalMap;
  typedef std::set<SessionTerminal *> TerminalSet;

 public:
  SessionServer(const core::InetAddress &address, const std::string &name);
  ~SessionServer();

  bool Initialize(const global::protocol::ServerSignature &signature,
      const core::InetAddress &admin_address);
  void Finalize();

  void Start();
  void Stop();

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
  void MulticastMessage(const T &message, gateway::protocol::MessageType::type type,
      core::uint64 actors[], size_t number);
  template <typename T>
  void BroadcastMessage(const T &message, gateway::protocol::MessageType::type type);

  // To game server.
  template <typename T>
  void BroadcastGameMessage(const T &message, protocol::GameMessageType::type type);

  // Terminal login/logout.
  bool RequestLogin(SessionTerminal *terminal);
  bool RequestLogout(SessionTerminal *terminal);

  void OnDisconnect(SessionTerminal *terminal);

  SessionTerminal *AllocateGameServer();
  SessionTerminal *GetTerminal(global::protocol::ServerType::type type, core::uint32 id);

  void ActorLogoutCenter(core::uint64 id);

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

  inline const core::TimeTick &GetTimeTick() const {
    return this->service_.GetTimeTick();
  }

 private:
  void OnConnection(const coresh::ConnectionPtr &connection);

  // join/disjoin schedule server.
  void OnServerJoinSchedule(const global::protocol::ServerSignature &signature);
  void OnServerDisjoinSchedule(const global::protocol::ServerSignature &signature);

  void OnDatabaseSessionLogined(const global::protocol::ServerSignature &signature);

  global::protocol::ServerSignature signature_;
  coresh::ApplicationService service_;

  schedule::client::ScheduleClientPtr schedule_client_;

  TerminalMap terminals_;
  TerminalSet connections_;

  GameServerIndex game_server_index_;

  global::ThriftPacket thrift_;
  global::ThriftPacket open_thrift_;
  global::WordFilter word_filter_;

  StorageResponse storage_response_;

  DayPass day_pass_;
};

template <typename T>
void SessionServer::BroadcastGateway(const T &message,
    protocol::MessageType::type type) {
  TerminalMap::iterator iterator = this->terminals_.begin();
  for(; iterator != this->terminals_.end(); ++iterator) {
    if(iterator->first.first == global::protocol::ServerType::GATEWAY_SERVER) {
      iterator->second->SendMessage(message, type);
    }
  }
}

template <typename T>
void SessionServer::MulticastMessage(const T &message,
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
  }
  this->BroadcastGateway(multicast,
      protocol::MessageType::MESSAGE_USER_MESSAGE_MULTICAST);
}

template <typename T>
void SessionServer::BroadcastMessage(const T &message,
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
void SessionServer::BroadcastGameMessage(const T &message,
    protocol::GameMessageType::type type) {
  TerminalMap::iterator iterator = this->terminals_.begin();
  for(; iterator != this->terminals_.end(); ++iterator) {
    if(iterator->first.first == global::protocol::ServerType::GAME_SERVER) {
      iterator->second->SendGameMessage(message, type);
    }
  }
}

class SessionServerSingleton : public core::Noncopyable {
 public:
  SessionServerSingleton() {}
  ~SessionServerSingleton() {}

  static inline void SetInstance(SessionServer *instance) {
    SessionServerSingleton::instance_ = instance;
  }
  static inline SessionServer &GetInstance() {
    return *SessionServerSingleton::instance_;
  }

 private:
  static SessionServer *instance_;
};

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__SESSION__SERVER__H

