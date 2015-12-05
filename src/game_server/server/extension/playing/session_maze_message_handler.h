#ifndef __GAME__SERVER__PLAYING__SESSION__MAZE__MESSAGE__HANDLER__H
#define __GAME__SERVER__PLAYING__SESSION__MAZE__MESSAGE__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"

namespace game {

namespace server {

namespace playing {

class SessionMazeMessageHandler : public core::Noncopyable {
 public:
  SessionMazeMessageHandler();
  ~SessionMazeMessageHandler();

  bool Initialize();
  void Finalize();
 
 private:
  // 地宫地图数据同步
  void OnMessageMazeMapInfoSynchronize(const char *data, size_t size);
};

}  // namespace playing

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__PLAYING__SESSION__MAZE__MESSAGE__HANDLER__H

