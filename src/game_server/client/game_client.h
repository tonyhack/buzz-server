//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-02-22 16:59:51.
// File name: game_client.h
//
// Description:
// Define class GameClient.
//

#ifndef __GAME__CLIENT__GAME__CLIENT__H
#define __GAME__CLIENT__GAME__CLIENT__H

#include <map>

#include <ext/hash_map>

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

#include "core/event_loop.h"
#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "game_server/protocol/game_protocol_types.h"
#include "global/server_type_types.h"
#include "global/thrift_packet.h"

namespace game {

namespace client {

class GameSession;

typedef boost::shared_ptr<GameSession> GameSessionPtr;

class GameClient : public core::Noncopyable {
  typedef std::map<core::uint32, GameSessionPtr> GameSessionMap;
  typedef boost::function<void (const GameSessionPtr &, const char *, size_t)> MessageHandler;

 public:
  GameClient();
  ~GameClient();

  bool Initialize(const global::protocol::ServerSignature &signature);
  void Finalize();

  void Start();
  void Stop();

  void OnGameServerJoin(core::EventLoop *network_loop,
      core::EventLoop *application_loop,
      const global::protocol::ServerSignature &signature);
  void OnGameServerDisjoin(const global::protocol::ServerSignature &signature);

  // Login/Logout.
  bool AddSession(const GameSessionPtr &session);
  bool StopSession(core::uint32 server_id);
  bool RemoveSession(core::uint32 server_id);

  // Get session.
  GameSessionPtr GetSession();
  GameSessionPtr GetSession(core::uint32 server_id);

  inline const global::protocol::ServerSignature &GetServerSignature() const {
    return this->signature_;
  }

  // Message handler.
  inline void SetMessageHandler(protocol::MessageType::type type,
      const MessageHandler &callback) {
    this->message_handlers_[type] = callback;
  }
  inline void ResetMessageHandler(protocol::MessageType::type type) {
    this->message_handlers_[type].clear();
  }

  void OnMessage(protocol::MessageType::type type, const GameSessionPtr &session,
      const char *message, size_t size);

 private:
  global::protocol::ServerSignature signature_;

  GameSessionMap sessions_;

  global::ThriftPacket thrift_;

  MessageHandler message_handlers_[protocol::MessageType::MESSAGE_MAX];
};

class GameClientSingleton : public core::Noncopyable {
 public:
  GameClientSingleton() {}
  ~GameClientSingleton() {}

  static inline GameClient &GetInstance() {
    return GameClientSingleton::instance_;
  }

 private:
  static GameClient instance_;
};

}  // namespace client

}  // namespace game

#endif  // __GAME__CLIENT__GAME__CLIENT__H

