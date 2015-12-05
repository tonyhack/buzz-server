//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-06 14:10:58.
// File name: task_message_handler.h
//
// Description:
// Define class TaskMessageHandler.
//

#ifndef __GAME__SERVER__TASK__TASK__MESSAGE__HANDLER__H
#define __GAME__SERVER__TASK__TASK__MESSAGE__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace task {

class TaskMessageHandler : public core::Noncopyable {
 public:
  TaskMessageHandler();
  ~TaskMessageHandler();

  bool Initialize();
  void Finalize();

 private:
  // 接受任务
  void OnMessageAcceptTask(core::uint64 id, const char *data, size_t size);
  // 完成任务
  void OnMessageFinishTask(core::uint64 id, const char *data, size_t size);
  // 放弃任务
  void OnMessageAbandonTask(core::uint64 id, const char *data, size_t size);
  // 与NPC对话
  void OnMessageTalkNpc(core::uint64 id, const char *data, size_t size);
  // 请求召唤NPC任务
  void OnMessageSummonNpc(core::uint64 id, const char *data, size_t size);
};

}  // namespace task

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TASK__TASK__MESSAGE__HANDLER__H

