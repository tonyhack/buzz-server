
#ifndef __GAME__SERVER__TASK__ROB__EVENT___HANDLER__H
#define __GAME__SERVER__TASK__ROB__EVENT___HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace task {

class RobEventHandler : public core::Noncopyable {
 public:
  RobEventHandler();
  ~RobEventHandler();

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

#endif  // __GAME__SERVER__TASK__ROB__EVENT___HANDLER__H

