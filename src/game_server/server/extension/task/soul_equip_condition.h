//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-10-09 22:21:41.
// File name: soul_equip_condition.h
//
// Description:
//
#ifndef __GAME__SERVER__TASK__SOUL__EQUIP__CONDITION__H
#define __GAME__SERVER__TASK__SOUL__EQUIP__CONDITION__H

#include "core/base/types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension/task/condition.h"

namespace game {

namespace server {

namespace task {

class SoulEquipCondition : public Condition {
 public:
  SoulEquipCondition();
  virtual ~SoulEquipCondition();

  virtual bool Initialize(Task *task, core::uint32 param1, core::uint32 param2,
      core::uint32 value = 0);
  virtual void Finalize();

  // 导出条件变量
  virtual bool ExportConditionVar(core::uint32 &var, core::uint32 &value);

  virtual entity::TaskCondition::type GetType() const {
    return entity::TaskCondition::EQUIP_SOUL;
  }

  virtual bool CheckCondition();
  virtual void Synchronize();

  virtual void OnEvent(event::EventType::type type,
      const void *data, size_t size);

 private:
  void OnEventSoulSet(const void *data, size_t size);

  // 英灵当前装备次数
  core::uint32 num_;

  // 英灵需要装备次数
  core::uint32 required_count_;
};

}  // namespace task

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TASK__SOUL__EQUIP__CONDITION__H

