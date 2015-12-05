//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-02-26 10:38:02.
// File name: game_message_handler.h
//
// Description:
// Define class GameMessageHandler.
//

#ifndef __SESSION__SERVER__GAME__MESSAGE__HANDLER__H
#define __SESSION__SERVER__GAME__MESSAGE__HANDLER__H

#include "core/base/noncopyable.h"
#include "global/logging.h"
#include "global/singleton_factory.h"
#include "global/thrift_packet.h"
#include "session_server/protocol/session_game_protocol_types.h"
#include "session_server/server/session_server.h"

namespace session {

namespace server {

class SessionTerminal;

class GameMessageHandler : public global::SingletonFactory<GameMessageHandler> {
  friend class global::SingletonFactory<GameMessageHandler>;

 public:
  typedef boost::function<void (SessionTerminal *terminal, const char *, size_t)>
    MessageHandler;

  bool Initialize();
  void Finalize();

  inline void SetMessageHandler(session::protocol::GameMessageType::type type,
      const MessageHandler &handler) {
    this->handler_[type] = handler;
  }
  inline void ResetMessageHandler(session::protocol::GameMessageType::type type) {
    this->handler_[type].clear();
  }

  void OnMessage(SessionTerminal *terminal, const char *message, size_t size);

 private:
  GameMessageHandler();
  ~GameMessageHandler();

  void OnMessageActorLoginResponse(SessionTerminal *terminal,
      const char *message, size_t size);

  void OnMessageActorBatchLogin(SessionTerminal *terminal,
      const char *message, size_t size);

  void OnMessageActorLevelSynchonize(SessionTerminal *terminal,
      const char *message, size_t size);

  void OnMessageActorVocationSynchronzie(SessionTerminal *terminal,
      const char *message, size_t size);

  void OnMessageActorFunctionalityStateSynchronize(SessionTerminal *terminal,
      const char *message, size_t size);

  void OnMessageActorFightScoreSychronize(SessionTerminal *terminal,
      const char *message, size_t size);

  void OnMessageActorCompareRequest(SessionTerminal *terminal,
      const char *message, size_t size);

  void OnMessageActorVipSynchronize(SessionTerminal *terminal,
      const char *message, size_t size);

  MessageHandler handler_[session::protocol::GameMessageType::MESSAGE_MAX];
};

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__GAME__MESSAGE__HANDLER__H

