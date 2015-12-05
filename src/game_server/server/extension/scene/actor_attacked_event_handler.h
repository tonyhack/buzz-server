#ifndef __GAME__SERVER__SCENE__ACTOR__ATTACKED__EVENT__HANDLER__H
#define __GAME__SERVER__SCENE__ACTOR__ATTACKED__EVENT__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace scene {

class ActorAttackedEventHandler {
 public:
  ActorAttackedEventHandler();
  ~ActorAttackedEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnEvent(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection conn_hurt_;

};

} // namespace scene

} // namepsace server

} // namepsace game

#endif  // __GAME__SERVER__SCENE__ACTOR__ATTACKED__EVENT__HANDLER__H
