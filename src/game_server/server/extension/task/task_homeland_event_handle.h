#ifndef __GAME__SERVER__TASK__TASK__HOMELAND__EVENT__HANDLER__H
#define __GAME__SERVER__TASK__TASK__HOMELAND__EVENT__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace task {

class TaskHomelandEventHandler {
 public:
  TaskHomelandEventHandler();
  ~TaskHomelandEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnEventWatering(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  void OnEventRob(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection conn_watering_;
  coresh::Communicator::Connection conn_rob_;
};

} // namespace task

} // namepsace server

} // namepsace game

#endif  // __GAME__SERVER__TASK__TASK__HOMELAND__EVENT__HANDLER__H
