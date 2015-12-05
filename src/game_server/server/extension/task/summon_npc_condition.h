//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2014-01-24 00:24:14.
// File name: summon_npc_condition.h
//
// Description:
//
#ifndef __GAME__SERVER__TASK__SUMMON__NPC__CONDITION__H
#define __GAME__SERVER__TASK__SUMMON__NPC__CONDITION__H

#include "core/base/types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension/task/condition.h"

namespace game {

namespace server {

namespace task {

class SummonNpcCondition : public Condition {
 public:
  SummonNpcCondition();
  virtual ~SummonNpcCondition();

  virtual bool Initialize(Task *task, core::uint32 param1, core::uint32 param2,
      core::uint32 value = 0);
  virtual void Finalize();

  virtual bool ExportConditionVar(core::uint32 &var, core::uint32 &value);

  virtual entity::TaskCondition::type GetType() const {
    return entity::TaskCondition::SUMMON_NPC;
  }

  virtual bool CheckCondition();
  virtual void Synchronize();

  virtual void OnEvent(event::EventType::type type,
      const void *data, size_t size);

  virtual void Clear();

  core::int32 GetTemplate() const {
    return this->require_id_;
  }

 private:
  void OnEvent(const void *data, size_t size);

  // 配置NPC ID
  core::int32 require_id_;

  // 击杀次数
  core::uint32 count_;

  // 配置文件次数
  core::int32 num_;
};

}  // namespace task

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TASK__SUMMON__NPC__CONDITION__H

