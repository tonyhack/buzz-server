//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-10-19 03:11:50.
// File name: actor_daily_clean_event_handler.h
//
// Description:
// Define class ActorDailyCleanEventHandler.
//

#ifndef __GAME__SERVER__TASK__ACTOR__DAILY__CLEAN__EVENT__HANDLER__H
#define __GAME__SERVER__TASK__ACTOR__DAILY__CLEAN__EVENT__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace task {

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

}  // namespace task

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TASK__ACTOR__DAILY__CLEAN__EVENT__HANDLER__H

