#ifndef __SESSION__SERVER__PLAYING__GAME__MAZE__MESSAGE__HANDLER__H
#define __SESSION__SERVER__PLAYING__GAME__MAZE__MESSAGE__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace session {

namespace server {

class SessionTerminal;

namespace playing {

class GameMazeMessageHandler : public core::Noncopyable {
 public:
  GameMazeMessageHandler();
  ~GameMazeMessageHandler();

  bool Initialize();
  void Finalize();

 private:
  // 地宫地图数据请求
  void OnMessageMazeMapInfoRequest(SessionTerminal *terminal, const char *data, size_t size);
};

}  // namespace playing

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__PLAYING__GAME__MAZE__MESSAGE__HANDLER__H

