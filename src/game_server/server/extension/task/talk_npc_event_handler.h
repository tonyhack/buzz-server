//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-16 11:43:52.
// File name: talk_npc_event_handler.h
//
// Description:
// Define class TalkNpcEventHandler.
//

#ifndef __GAME__SERVER__TASK__TALK__NPC__EVENT__HANDLER__H
#define __GAME__SERVER__TASK__TALK__NPC__EVENT__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace task {

class TalkNpcEventHandler : public core::Noncopyable {
 public:
  TalkNpcEventHandler();
  ~TalkNpcEventHandler();

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

#endif  // __GAME__SERVER__TASK__TALK__NPC__EVENT__HANDLER__H

