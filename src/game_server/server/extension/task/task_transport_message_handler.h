//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-06-12 03:33:01.
// File name: task_transport_message_handler.h
//
// Description:
// Define class TaskTransportMessageHandler.
//

#ifndef __GAME__SERVER__TASK__TASK__TRANSPORT__MESSAGE__HANDLER__H
#define __GAME__SERVER__TASK__TASK__TRANSPORT__MESSAGE__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace task {

class TaskTransportMessageHandler : public core::Noncopyable {
 public:
  TaskTransportMessageHandler();
  ~TaskTransportMessageHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnMessageTaskTransportRefreshQualityRequest(core::uint64 id,
      const char *data, size_t size);
  void OnMessageTaskTransportAcceptRequest(core::uint64 id,
      const char *data, size_t size);
  void OnMessageTaskTransportFinishRequest(core::uint64 id,
      const char *data, size_t size);
};

}  // namespace task

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TASK__TASK__TRANSPORT__MESSAGE__HANDLER__H

