//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-11 18:24:27.
// File name: actor_levelup_event_handler.h
//
// Description:
// Define class ActorLevelupEventHandler.
//

#ifndef __SESSION__SERVER__TEAM__ACTOR__LEVELUP__EVENT__HANDLER__H
#define __SESSION__SERVER__TEAM__ACTOR__LEVELUP__EVENT__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace session {

namespace server {

namespace team {

class ActorLevelupEventHandler : public core::Noncopyable {
 public:
  ActorLevelupEventHandler();
  ~ActorLevelupEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnEvent(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection connection_;
};

}  // namespace team

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__TEAM__ACTOR__LEVELUP__EVENT__HANDLER__H

