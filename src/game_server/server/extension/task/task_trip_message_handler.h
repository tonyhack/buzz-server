//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-10-24 23:12:56.
// File name: task_trip_message_handler.h
//
// Description:
//
#ifndef __GAME__SERVER__TASK__TASK__TRIP__MESSAGE__HANDLER__H
#define __GAME__SERVER__TASK__TASK__TRIP__MESSAGE__HANDLER__H
#include <stddef.h> 

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace task {

class TaskTripMessageHandler : public core::Noncopyable {
 public:
  TaskTripMessageHandler();
  ~TaskTripMessageHandler();

  bool Initialize();
  void Finalize();

 private:
  // 奇遇任务操作类型
  void OnMessageOpt(core::uint64 id, const char *data, size_t size);
};

} // namespace task

} // namespace server

} // namespace game

#endif //__GAME__SERVER__TASK__TASK__TRIP__MESSAGE__HANDLER__H
