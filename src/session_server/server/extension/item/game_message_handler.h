#ifndef __SESSION__SERVER__ITEM__GAME__MESSAGE__HANDLER__H
#define __SESSION__SERVER__ITEM__GAME__MESSAGE__HANDLER__H

#include "core/base/noncopyable.h"
#include "session_server/server/session_terminal.h"

namespace session {

namespace server {

namespace item {

class GameMessageHandler {
 public:
  GameMessageHandler();
  ~GameMessageHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnTreasureHuntSynchronize(SessionTerminal *terminal, 
      const char *data, size_t size);
};

} // namespace item

} // namespace server

} // namespace session

#endif //  __SESSION__SERVER__ITEM__GAME__MESSAGE__HANDLER__H

