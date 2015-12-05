//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-31 15:27:02.
// File name: gateway_server.h
//
// Description:
// Define class GatewayServer.
//

#ifndef __GATEWAY__SERVER__GATEWAY__SERVER__H
#define __GATEWAY__SERVER__GATEWAY__SERVER__H

#include <map>
#include <set>

#include "core/async_logging_singleton.h"
#include "core/inet_address.h"
#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/application_service.h"
#include "gateway_server/server/game_message_handler.h"
#include "gateway_server/server/session_message_handler.h"
#include "gateway_server/server/storage_response_handler.h"
#include "global/kickout_actor_type_types.h"
#include "global/random_generator.h"
#include "global/server_type_types.h"
#include "global/thrift_packet.h"
#include "schedule_server/client/schedule_client.h"
#include "session_server/client/session_client.h"

namespace gateway {

namespace server {

class GatewayUser;

class GatewayServer : public core::Noncopyable {
  typedef std::set<GatewayUser *> ConnectUserSet;

 public:
  typedef std::map<core::uint64, GatewayUser *> LoginUserMap;

  GatewayServer(const core::InetAddress &address, const std::string &name);
  ~GatewayServer();

  bool Initialize(const global::protocol::ServerSignature &signature);
  void Finalize();

  void Start();
  void Stop();

  void OnDisconnect(GatewayUser *user);
  bool Login(GatewayUser *user);
  bool Logout(GatewayUser *user);

  void KickOut(core::uint64 id, global::protocol::KickoutActorType::type reason);

  GatewayUser *GetUser(core::uint64 id);

  inline LoginUserMap &GetUsers() {
    return this->login_users_;
  }

  inline void AddTimer(core::uint32 id, core::uint32 msecs, int call_times,
      core::TimerHandler *handler, const std::string &debug) {
    this->service_.AddTimer(id, msecs, call_times, handler, debug);
  }
  inline void RemoveTimer(core::uint32 id, core::TimerHandler *handler) {
    this->service_.RemoveTimer(id, handler);
  }

  inline global::ThriftPacket *GetThrift() {
    return &this->thrift_;
  }
  inline global::RandomGenerator *GetRandomGenerator() {
    return &this->random_;
  }

  inline const global::protocol::ServerSignature &GetSignature() {
    return this->signature_;
  }
  inline void SetSignature(const global::protocol::ServerSignature &signature) {
    this->signature_ = signature;
  }

  inline session::client::SessionClientPtr &GetSessionClient() {
    return this->session_client_;
  }

 private:
  void OnConnection(const coresh::ConnectionPtr &connection);

  void OnServerJoinSchedule(const global::protocol::ServerSignature &signature);
  void OnServerDisjoinSchedule(const global::protocol::ServerSignature &signature);

  void OnGameJoin(const global::protocol::ServerSignature &signature);
  void OnGameDisjoin(const global::protocol::ServerSignature &signature);

  void OnDatabaseJoin(const global::protocol::ServerSignature &signature);
  void OnDatabaseDisjoin(const global::protocol::ServerSignature &signature);

  void OnSessionJoin(const global::protocol::ServerSignature &signature);
  void OnSessionDisjoin(const global::protocol::ServerSignature &signature);

  global::protocol::ServerSignature signature_;
  coresh::ApplicationService service_;

  schedule::client::ScheduleClientPtr schedule_client_;
  session::client::SessionClientPtr session_client_;

  GameMessagehandler game_message_handler_;
  SessionMessageHandler session_message_handler_;

  global::ThriftPacket thrift_;
  global::ThriftPacket open_thrift_;

  global::RandomGenerator random_;

  ConnectUserSet connect_users_;
  LoginUserMap login_users_;

  StorageResponseHandler storage_response_handler_;
};

class GatewayServerSingleton : public core::Noncopyable {
 public:
  GatewayServerSingleton() {}
  ~GatewayServerSingleton() {}

  static inline void SetInstance(GatewayServer *instance) {
    GatewayServerSingleton::instance_ = instance;
  }
  static inline GatewayServer &GetInstance() {
    return *GatewayServerSingleton::instance_;
  }

 private:
  static GatewayServer *instance_;
};

}  // namespace server

}  // namespace gateway

#endif  // __GATEWAY__SERVER__GATEWAY__SERVER__H

