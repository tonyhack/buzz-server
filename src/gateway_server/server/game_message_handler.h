//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-02-25 10:27:26.
// File name: game_message_handler.h
//
// Description:
// Define class GameMessagehandler.
//

#ifndef __GATEWAY__SERVER__GAME__MESSAGE__HANDLER__H
#define __GATEWAY__SERVER__GAME__MESSAGE__HANDLER__H

#include "core/base/noncopyable.h"
#include "game_server/client/game_session.h"
#include "global/thrift_packet.h"

namespace gateway {

namespace server {

class GameMessagehandler : public core::Noncopyable {
 public:
  GameMessagehandler();
  ~GameMessagehandler();

  bool Initialize();
  void Finalize();

 private:
  void OnMessage(const game::client::GameSessionPtr &session,
      const char *message, size_t size);

  void OnUserMessageForward(const game::client::GameSessionPtr &session,
      const char *message, size_t size);
  void OnUserMessageBroadcast(const game::client::GameSessionPtr &session,
      const char *message, size_t size);
  void OnUserMessageMulticast(const game::client::GameSessionPtr &session,
      const char *message, size_t size);

  global::ThriftPacket thrift_;
};

}  // namespace server

}  // namespace gateway

#endif  // __GATEWAY__SERVER__GAME__MESSAGE__HANDLER__H

