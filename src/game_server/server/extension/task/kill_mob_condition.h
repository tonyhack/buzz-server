//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-05 11:38:17.
// File name: kill_mob_condition.h
//
// Description:
// Define class KillMobCondition.
//

#ifndef __GAME__SERVER__TASK__KILL_MOB__CONDITION__H
#define __GAME__SERVER__TASK__KILL_MOB__CONDITION__H

#include "core/base/types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension/task/condition.h"

namespace game {

namespace server {

namespace task {

class KillMobCondition : public Condition {
 public:
  KillMobCondition();
  virtual ~KillMobCondition();

  virtual bool Initialize(Task *task, core::uint32 param1, core::uint32 param2,
      core::uint32 value = 0);
  virtual void Finalize();

  // 导出条件变量
  virtual bool ExportConditionVar(core::uint32 &var, core::uint32 &value);

  virtual entity::TaskCondition::type GetType() const {
    return entity::TaskCondition::KILL_MOB;
  }

  virtual bool CheckCondition();
  virtual void Synchronize();

  virtual void OnEvent(event::EventType::type type,
      const void *data, size_t size);

 private:
  // 杀死怪物回调
  void OnEventKillMob(const void *data, size_t size);

  // 怪物ID
  core::uint32 npc_;
  // 怪物数量
  core::uint32 require_kill_;

  // 已经杀死数量
  core::uint32 killed_;
};

}  // namespace task

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TASK__KILL_MOB__CONDITION__H

