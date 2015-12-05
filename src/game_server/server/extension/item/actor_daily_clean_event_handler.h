//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2014-02-22 03:33:40.
// File name: actor_daily_clean_event_handler.h
//
// Description:
//

#ifndef __GAME__SERVER__ITEM__ACTOR__DAILY__CLEAN__EVENT__HANDLER__H
#define __GAME__SERVER__ITEM__ACTOR__DAILY__CLEAN__EVENT__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace item {

class ActorDailyCleanEventHandler : public core::Noncopyable {
 public:
  ActorDailyCleanEventHandler();
  ~ActorDailyCleanEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnEvent(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection connection_;
};

}  // namespace item

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ITEM__ACTOR__DAILY__CLEAN__EVENT__HANDLER__H

