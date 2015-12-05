#ifndef __GAME__SERVER__TASK__LOAD__TASK__DAILY__CONFIGURE__H
#define __GAME__SERVER__TASK__LOAD__TASK__DAILY__CONFIGURE__H

#include <map>
#include <string>
#include <vector>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "game_server/server/extension/task/task_configure.h"

namespace game {

namespace server {

namespace task {

class TaskDailyCell {
 public:
  core::int32 id_;
  core::int32 award_;   // 声望
  TaskCondition condition_;
  core::int32 front_id_;
  core::int32 spend_;
};

class LoadTaskDailyConfigure : public core::Noncopyable {
 public:
  typedef std::map<core::int32, TaskDailyCell> TaskDailyMap;

  LoadTaskDailyConfigure();
  ~LoadTaskDailyConfigure();

  bool LoadConfigure(const std::string &file);

  const TaskDailyCell *GetCell(const core::int32 task_id) const;

  const TaskDailyMap &GetDailyList() const;

 private:
  TaskDailyMap tasks_;
};

}  // namespace task

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TASK__LOAD__TASK__DAILY__CONFIGURE__H

