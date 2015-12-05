//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-12-14 03:07:22.
// File name: world_jump_message_handler.h
//
// Description:
//

#ifndef __GAME__SERVER__SCENE__WORLD__JUMP__MESSAGE__HANDLER__H
#define __GAME__SERVER__SCENE__WORLD__JUMP__MESSAGE__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "game_server/server/game_actor.h"

namespace game {

namespace server {

namespace scene {

class WorldJumpMessageHandler : public core::Noncopyable {
 public:
  WorldJumpMessageHandler();
  ~WorldJumpMessageHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnRequest(core::uint64 id, const char *data, size_t size);
  bool JumpMap(GameActor *actor, core::int32 map, core::int32 x, core::int32 y);
};

}  // namespace scene

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SCENE__WORLD__JUMP__MESSAGE__HANDLER__H

