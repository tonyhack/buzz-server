//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-11-27 20:47:33.
// File name: bar_finish_event_handler.h
//
// Description:
//
#ifndef __GAME__SERVER__ATTR__BAR__FINISH__EVENT__HANDLER__H
#define __GAME__SERVER__ATTR__BAR__FINISH__EVENT__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

class GameActor;

namespace attr {

class BarFinishEventHandler {
 public:
  BarFinishEventHandler();
  ~BarFinishEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnEvent(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnTransformedEvent(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void NoticeSpiritTransFormed(GameActor *actor, bool result);

  coresh::Communicator::Connection conn_spirit_transformed_;

};

} // namespace attr

} // namepsace server

} // namepsace game

#endif  // __GAME__SERVER__ATTR__BAR__FINISH__EVENT__HANDLER__H
