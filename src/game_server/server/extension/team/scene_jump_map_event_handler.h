//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-01-03 19:20:10.
// File name: scene_jump_map_event_handler.h
//
// Description:
// Define class SceneJumpMapEventHandler.
//

#ifndef __GAME__SERVER__TEAM__SCENE__JUMP__MAP__EVENT__HANDLER__H
#define __GAME__SERVER__TEAM__SCENE__JUMP__MAP__EVENT__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace team {

class SceneJumpMapEventHandler : public core::Noncopyable {
 public:
  SceneJumpMapEventHandler();
  ~SceneJumpMapEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnEvent(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection connection_;
};

}  // namespace team

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TEAM__SCENE__JUMP__MAP__EVENT__HANDLER__H

