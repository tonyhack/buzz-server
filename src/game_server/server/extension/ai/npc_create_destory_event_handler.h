//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-09 18:34:48.
// File name: npc_create_destory_event_handler.h
//
// Description:
// Define class NpcCreateDestoryEventHandler.
//

#ifndef __GAME__SERVER__AI__NPC__CREATE__DESTORY__EVENT__HANDLER__H
#define __GAME__SERVER__AI__NPC__CREATE__DESTORY__EVENT__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace ai {

class NpcCreateDestoryEventHandler : public core::Noncopyable {
 public:
  NpcCreateDestoryEventHandler();
  ~NpcCreateDestoryEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnCreateEvent(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnDestoryEvent(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection connection_create_;
  coresh::Communicator::Connection connection_destory_;
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__NPC__CREATE__DESTORY__EVENT__HANDLER__H

