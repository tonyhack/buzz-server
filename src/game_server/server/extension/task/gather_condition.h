//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-11-07 03:55:37.
// File name: gather_condition.h
//
// Description:
//
#ifndef __GAME__SERVER__TASK__GATHER__CONDITION__H
#define __GAME__SERVER__TASK__GATHER__CONDITION__H

#include "core/base/types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension/task/condition.h"

namespace game {

namespace server {

namespace task {

class GatherCondition : public Condition {
 public:
  GatherCondition();
  virtual ~GatherCondition();

  virtual bool Initialize(Task *task, core::uint32 param1, core::uint32 param2,
      core::uint32 value = 0);
  virtual void Finalize();

  virtual bool ExportConditionVar(core::uint32 &var, core::uint32 &value);

  virtual entity::TaskCondition::type GetType() const {
    return entity::TaskCondition::PICK;
  }

  virtual bool CheckCondition();
  virtual void Synchronize();

  virtual void OnEvent(event::EventType::type type,
      const void *data, size_t size);
  
  virtual void Clear();
  
  core::int32 GetTemplateID() {
    return this->require_id_;
  }

 private:
  void OnEventCollect(const void *data, size_t size);

  // 配置ID
  core::int32 require_id_;

  // 需要道具的数目
  core::uint32 count_;
};

}  // namespace task

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TASK__GATHER__CONDITION__H

