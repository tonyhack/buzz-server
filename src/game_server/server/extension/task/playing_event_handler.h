//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-10-12 18:22:53.
// File name: playing_event_handler.h
//
// Description:
//
#ifndef __GAME__SERVER__TASK__PLAYING__EVENT__HANDLER__H
#define __GAME__SERVER__TASK__PLAYING__EVENT__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace task {

class PlayingEventHandler : public core::Noncopyable {
 public:
  PlayingEventHandler();
  ~PlayingEventHandler();

  bool Initialize();
  void Finalize();

 private:

  // 道具使用
  void OnFinishEvent(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection conn_finish_;
};

}  // namespace task

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TASK__PLAYING__EVENT__HANDLER__H

