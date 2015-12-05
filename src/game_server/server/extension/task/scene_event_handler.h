//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-11-07 03:39:49.
// File name: scene_event_handler.h
//
// Description:
//
#ifndef __GAME__SERVER__TASK__SCENE__EVENT__HANDLER__H
#define __GAME__SERVER__TASK__SCENE__EVENT__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace task {

class SceneEventHandler {
 public:
  SceneEventHandler();
  ~SceneEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnEvent(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection conn_gather_;

};

} // namespace task

} // namepsace server

} // namepsace game

#endif  // __GAME__SERVER__TASK__SCENE__EVENT__HANDLER__H
