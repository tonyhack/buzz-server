//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-16 17:59:05.
// File name: soul_appear_event_handler.h
//
// Description:
// Define class SoulAppearEventHandler.
//

#ifndef __GAME__SERVER__TASK__SOUL__APPEAR__EVENT___HANDLER__H
#define __GAME__SERVER__TASK__SOUL__APPEAR__EVENT___HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace task {

class SoulAppearEventHandler : public core::Noncopyable {
 public:
  SoulAppearEventHandler();
  ~SoulAppearEventHandler();

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

#endif  // __GAME__SERVER__TASK__SOUL__APPEAR__EVENT___HANDLER__H

