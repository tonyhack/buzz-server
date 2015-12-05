//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-06-12 01:51:22.
// File name: load_task_transport_configure.h
//
// Description:
// Define class LoadTaskTransportConfigure.
//

#ifndef __GAME__SERVER__TASK__LOAD__TASK__TRANSPORT__CONFIGURE__H
#define __GAME__SERVER__TASK__LOAD__TASK__TRANSPORT__CONFIGURE__H

#include <string>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "game_server/server/extension/task/task_configure.h"

namespace game {

namespace server {

namespace task {

class LoadTaskTransportConfigure : public core::Noncopyable {
  typedef std::vector<TaskTransportCell> TransportVector;

 public:
  LoadTaskTransportConfigure();
  ~LoadTaskTransportConfigure();

  bool LoadConfigure(const std::string &file);

  const TaskTransportCell *GetTransport(core::int32 id) const;
  const TaskTransportCell *Random() const;

 private:
  TransportVector transports_;
};

}  // namespace task

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TASK__LOAD__TASK__TRANSPORT__CONFIGURE__H

