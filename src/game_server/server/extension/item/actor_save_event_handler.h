//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-24 14:42:48.
// File name: actor_save_event_handler.h
//
// Description:
// Define class ActorSaveEventHandler.
//

#ifndef __GAME__SERVER__ITEM__ACTOR__SAVE__EVENT__HANDLER__H
#define __GAME__SERVER__ITEM__ACTOR__SAVE__EVENT__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace item {

class ActorSaveEventHandler : public core::Noncopyable {
 public:
  ActorSaveEventHandler();
  ~ActorSaveEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnEvent(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection conn_actor_save_;
};

}  // namespace item

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ITEM__ACTOR__SAVE__EVENT__HANDLER__H

