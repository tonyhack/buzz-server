//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-05 14:20:51.
// File name: load_task_configure.h
//
// Description:
// Define class LoadTaskConfigure.
//

#ifndef __GAME__SERVER__TASK__LOAD__TASK__CONFIGURE__H
#define __GAME__SERVER__TASK__LOAD__TASK__CONFIGURE__H

#include <ext/hash_map>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "game_server/server/extension/task/task_configure.h"

namespace game {

namespace server {

namespace task {

class LoadTaskConfigure : public core::Noncopyable {
  typedef __gnu_cxx::hash_map<core::uint32, TaskCell> TaskHashmap;

 public:
  LoadTaskConfigure();
  ~LoadTaskConfigure();

  bool LoadConfigure(const std::string &file);

  const TaskCell *GetTask(core::uint32 id) const;

 private:
  TaskHashmap tasks_;
};

}  // namespace task

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TASK__LOAD__TASK__CONFIGURE__H

