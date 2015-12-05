//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-05 11:08:09.
// File name: condition.h
//
// Description:
// Define class Condition.
//

#ifndef __GAME__SERVER__TASK__CONDITION__H
#define __GAME__SERVER__TASK__CONDITION__H

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "entity/task_types.h"
#include "game_server/server/event/game_event_types.h"

namespace game {

namespace server {

namespace task {

class Task;

class Condition : public core::Noncopyable {
 public:
  Condition();
  virtual ~Condition();

  virtual bool Initialize(Task *task, core::uint32 param1, core::uint32 param2,
      core::uint32 value = 0);
  virtual void Finalize();

  // 导出条件变量
  virtual bool ExportConditionVar(core::uint32 &var, core::uint32 &value) = 0;

  virtual entity::TaskCondition::type GetType() const = 0;

  // 条件检测
  virtual bool CheckCondition() = 0;

  // 事件回调
  virtual void OnEvent(event::EventType::type type,
      const void *data, size_t size) {}

  // 同步
  virtual void Synchronize() = 0;

  // 条件完成做的处理
  virtual void Clear() {}

  virtual void OnFinish() {}
  virtual void OnAbandon() {}

 protected:
  Task *task_;
};

}  // namespace task

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TASK__CONDITION__H

