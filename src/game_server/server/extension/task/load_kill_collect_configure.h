//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-05 15:31:35.
// File name: load_kill_collect_configure.h
//
// Description:
// Define class LoadKillCollectConfigure.
//

#ifndef __GAME__SERVER__TASK__LOAD__KILL__COLLECT__CONFIGURE__H
#define __GAME__SERVER__TASK__LOAD__KILL__COLLECT__CONFIGURE__H

#include <map>
#include <string>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "game_server/server/extension/task/task_configure.h"

namespace game {

namespace server {

namespace task {

class LoadKillCollectConfigure : public core::Noncopyable {
  typedef std::map<core::uint32, KillCollectCell> KillCollectmap;

 public:
  LoadKillCollectConfigure();
  ~LoadKillCollectConfigure();

  bool LoadConfigure(const std::string &file);

  const KillCollectCell *GetKillCollect(const core::uint32 key) const;

 private:
  KillCollectmap cells_;
};

}  // namespace task

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TASK__LOAD__KILL__COLLECT__CONFIGURE__H

