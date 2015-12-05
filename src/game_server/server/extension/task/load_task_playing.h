#ifndef __GAME__SERVER__TASK__LOAD__TASK__PLAYING__CONFIGURE__H
#define __GAME__SERVER__TASK__LOAD__TASK__PLAYING__CONFIGURE__H

#include <map>
#include <vector>
#include <ext/hash_map>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "game_server/server/extension/task/task_configure.h"

namespace game {

namespace server {

namespace task {

class TaskPlayingCell {
 public:
  std::vector<core::int32> maps_;
};

class LoadTaskPlayingConfigure : public core::Noncopyable {
  typedef std::map<core::int32, TaskPlayingCell> Maps;
 public:
  LoadTaskPlayingConfigure();
  ~LoadTaskPlayingConfigure();

  bool LoadConfigure(const std::string &file);

  bool Check(core::int32 id, core::int32 map_id) const;

 private:
  Maps maps_;
};

}  // namespace task

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TASK__LOAD__TASK__PLAYING__CONFIGURE__H
