//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-10-11 01:40:00.
// File name: level_up_condition.h
//
// Description:
//
#ifndef __GAME__SERVER__TASK__LEVEL__UP__CONDITION__H
#define __GAME__SERVER__TASK__LEVEL__UP__CONDITION__H

#include "core/base/types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension/task/condition.h"

namespace game {

namespace server {

namespace task {

class LevelUpCondition : public Condition {
 public:
  LevelUpCondition();
  virtual ~LevelUpCondition();

  virtual bool Initialize(Task *task, core::uint32 param1, core::uint32 param2,
      core::uint32 value = 0);
  virtual void Finalize();

  virtual bool ExportConditionVar(core::uint32 &var, core::uint32 &value);

  virtual entity::TaskCondition::type GetType() const {
    return entity::TaskCondition::ROLE_LV;
  }

  virtual bool CheckCondition();
  virtual void Synchronize();
  virtual void OnEvent(event::EventType::type type,
      const void *data, size_t size);

 private:
  void OnEventLevelUp(const void *data, size_t size);
  // 配置等级
  core::int32 require_lv_;  

};

}  // namespace task

}  // namespace server

}  // namespace game

#endif // __GAME__SERVER__TASK__LEVEL__UP__CONDITION__H

