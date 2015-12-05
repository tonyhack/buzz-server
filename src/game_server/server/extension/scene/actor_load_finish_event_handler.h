//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-09 17:36:22.
// File name: actor_load_finish_event_handler.h
//
// Description:
// Define class ActorLoadFinishEventHandler.
//

#ifndef __GAME__SERVER__SCENE__ACTOR__LOAD__FINISH__EVENT__HANDLER__H
#define __GAME__SERVER__SCENE__ACTOR__LOAD__FINISH__EVENT__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace scene {

class ActorLoadFinishEventHandler : public core::Noncopyable {
 public:
  ActorLoadFinishEventHandler();
  ~ActorLoadFinishEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnEvent(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection connection_;
};

}  // namespace scene

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SCENE__ACTOR__LOAD__FINISH__EVENT__HANDLER__H

