#ifndef __GAME__SERVER__PLAYING__ACTOR__WEEKLY__CLEAN__EVENT__HANDLER__H
#define __GAME__SERVER__PLAYING__ACTOR__WEEKLY__CLEAN__EVENT__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace playing {

class ActorWeeklyCleanEventHandler : public core::Noncopyable {
 public:
  ActorWeeklyCleanEventHandler();
  ~ActorWeeklyCleanEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnEvent(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection connection_;
};

}  // namespace playing

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__PLAYING__ACTOR__WEEKLY__CLEAN__EVENT__HANDLER__H

