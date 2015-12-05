//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-26 16:24:36.
// File name: game_message_handler.h
//
// Description:
// Define class GameMessageHandler.
//

#ifndef __SESSION__SERVER__SCENE__GAME__MESSAGE__HANDLER__H
#define __SESSION__SERVER__SCENE__GAME__MESSAGE__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "session_server/server/session_terminal.h"

namespace session {

namespace server {

namespace scene {

class GameMessageHandler : public core::Noncopyable {
 public:
  GameMessageHandler();
  ~GameMessageHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnMessageSceneLogin(SessionTerminal *terminal, const char *data,
      size_t size);
  void OnMessageSceneBatchLogin(SessionTerminal *terminal,
      const char *data, size_t size);
  void OnMessageSceneLogoutRequest(SessionTerminal *terminal,
      const char *data, size_t size);
  void OnMessageSceneWorldBossKilled(SessionTerminal *terminal,
      const char *data, size_t size);
  void OnMessageSceneJumpSceneRequest(SessionTerminal *terminal,
      const char *data, size_t size);
  void OnMessageSceneActorJumpMapRequest(SessionTerminal *terminal,
      const char *data, size_t size);
  void OnMessageSceneActorJumpLoginRequest(SessionTerminal *terminal,
      const char *data, size_t size);
  void OnMessageSceneActorJumpLogoutRequest(SessionTerminal *terminal,
      const char *data, size_t size);
};

}  // namespace scene

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__SCENE__GAME__MESSAGE__HANDLER__H

