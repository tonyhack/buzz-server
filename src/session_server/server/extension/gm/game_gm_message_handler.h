#ifndef __SESSION__SERVER__GM__GAME__GM__MESSAGE__HANDLER__H
#define __SESSION__SERVER__GM__GAME__GM__MESSAGE__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "session_server/server/session_terminal.h"

namespace session {

namespace server {

class SessionTerminal;

namespace gm {

class GameGmMessageHandler : public core::Noncopyable {
 public:
  GameGmMessageHandler();
  ~GameGmMessageHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnMessageGmCommand(SessionTerminal *terminal, const char *data, size_t size);
};

}  // namespace gm

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__GM__GAME__GM__MESSAGE__HANDLER__H

