//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-11 14:35:05.
// File name: game_message_handler.h
//
// Description:
// Define class GameMessageHandler.
//

#ifndef __SESSION__SERVER__TEAM__GAME__MESSAGE__HANDLER__H
#define __SESSION__SERVER__TEAM__GAME__MESSAGE__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "session_server/server/session_terminal.h"

namespace session {

namespace server {

namespace team {

class GameMessageHandler : public core::Noncopyable {
 public:
  GameMessageHandler();
  ~GameMessageHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnMessageTeamMemberAttrSynchronize(SessionTerminal *terminal, const char *data, size_t size);
};

}  // namespace team

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__TEAM__GAME__MESSAGE__HANDLER__H

