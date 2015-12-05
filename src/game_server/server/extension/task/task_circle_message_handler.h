//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-10-14 19:34:56.
// File name: task_circle_message_handler.h
//
// Description:
//
#ifndef __GAME__SERVER__TASK__TASK__CIRCLE__MESSAGE__HANDLER__H
#define __GAME__SERVER__TASK__TASK__CIRCLE__MESSAGE__HANDLER__H
#include <stddef.h> 

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "game_server/server/extension/task/configure.h"
#include "game_server/server/extension/task/load_task_circle_configure.h"
#include "game_server/server/game_actor.h"

namespace game {

namespace server {

namespace task {

class TaskCircleMessageHandler : public core::Noncopyable {
 public:
  TaskCircleMessageHandler();
  ~TaskCircleMessageHandler();

  bool Initialize();
  void Finalize();

 private:
   // 循环任务操作类型
   void OnMessageOpt(core::uint64 id, const char *data, size_t size);
   // 循环任务奖励
   bool Award(GameActor *game_actor, const TaskCircleCell *cell, 
      const ConditionCell * conditionCell, bool round, core::int32 number);

};

} // namespace task

} // namespace server

} // namespace game

#endif //__GAME__SERVER__TASK__TASK__CIRCLE__MESSAGE__HANDLER__H
