#ifndef __GAME__SERVER__PLAYING__MAZE__MESSAGE__HANDLER__H
#define __GAME__SERVER__PLAYING__MAZE__MESSAGE__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace playing {

class MazeMessageHandler : public core::Noncopyable {
 public:
  MazeMessageHandler();
  ~MazeMessageHandler();

  bool Initialize();
  void Finalize();

 private:
  // 地宫移动请求
  void OnMessageMazeMoveRequest(core::uint64 id, const char *data, size_t size);
  // 地宫确认移动请求
  void OnMessageMazeConfirmMoveRequest(core::uint64 id, const char *data, size_t size);
  // 地宫领取奖励请求
  void OnMessageMazeAwardRequest(core::uint64 id, const char *data, size_t size);
};

}  // namespace playing

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__PLAYING__MAZE__MESSAGE__HANDLER__H

