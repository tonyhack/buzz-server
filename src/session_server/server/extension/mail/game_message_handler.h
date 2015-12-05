//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-09-13 20:08:41.
// File name: game_message_handler.h
//
// Description:
//
#ifndef __SESSION__SERVER__MAIL__GAME__MESSAGE__HANDLER__H
#define __SESSION__SERVER__MAIL__GAME__MESSAGE__HANDLER__H

#include "core/base/noncopyable.h"
#include "session_server/server/session_terminal.h"

namespace session {

namespace server {

namespace mail {

class GameMessageHandler {
 public:
  GameMessageHandler();
  ~GameMessageHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnMessageMailSendRequest(SessionTerminal *terminal, 
      const char *data, size_t size);
};

} // namespace mail

} // namespace server

} // namespace session

#endif //  __SESSION__SERVER__MAIL__GAME__MESSAGE__HANDLER__H

