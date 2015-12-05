//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-10-16 00:08:54.
// File name: functionality_event_handler.h
//
// Description:
//
#ifndef __GAME__SERVER__TASK__FUNCTIONALITY__EVENT__HANDLER__H
#define __GAME__SERVER__TASK__FUNCTIONALITY__EVENT__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace task {

class FunctionalityEventHandler : public core::Noncopyable {
 public:
  FunctionalityEventHandler();
  ~FunctionalityEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnEvent(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection conn_functionality_;
};

}  // namespace task

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TASK__FUNCTIONALITY__EVENT__HANDLER__H
