#ifndef __GAME__SERVER__TASK__TASK__DAILY__MESSAGE__HANDLER__H
#define __GAME__SERVER__TASK__TASK__DAILY__MESSAGE__HANDLER__H
#include <stddef.h> 

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "game_server/server/extension/task/configure.h"
#include "game_server/server/extension/task/load_task_circle_configure.h"
#include "game_server/server/game_actor.h"

namespace game {

namespace server {

namespace task {

class TaskDailyMessageHandler : public core::Noncopyable {
 public:
  TaskDailyMessageHandler();
  ~TaskDailyMessageHandler();

  bool Initialize();
  void Finalize();

 private:
   // 每日任务完成
   void OnTask(core::uint64 id, const char *data, size_t size);
   // 奖励
   void OnAward(core::uint64 id, const char *data, size_t size);

};

} // namespace task

} // namespace server

} // namespace game

#endif //__GAME__SERVER__TASK__TASK__DAILY__MESSAGE__HANDLER__H
