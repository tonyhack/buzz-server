//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-11-11 01:33:55.
// File name: scene_finish_event_handler.h
//
// Description:
//
#ifndef __GAME__SERVER__SCENE__BAR__FINISH__EVENT__HANDLER__H
#define __GAME__SERVER__SCENE__BAR__FINISH__EVENT__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

class GameActor;

namespace scene {

class BarFinishEventHandler {
 public:
  BarFinishEventHandler();
  ~BarFinishEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnEvent(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void NoticeGatherResult(GameActor *actor, core::int32 npc = 0);

  coresh::Communicator::Connection conn_bar_finish_;

};

} // namespace scene

} // namepsace server

} // namepsace game

#endif  // __GAME__SERVER__SCENE__BAR__FINISH__EVENT__HANDLER__H
