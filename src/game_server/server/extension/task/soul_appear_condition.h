//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-16 17:35:39.
// File name: soul_appear_condition.h
//
// Description:
// Define class SoulAppearCondition.
//

#ifndef __GAME__SERVER__TASK__SOUL__APPEAR__CONDITION__H
#define __GAME__SERVER__TASK__SOUL__APPEAR__CONDITION__H

#include "core/base/types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension/task/condition.h"

namespace game {

namespace server {

namespace task {

class SoulAppearCondition : public Condition {
 public:
  SoulAppearCondition();
  virtual ~SoulAppearCondition();

  virtual bool Initialize(Task *task, core::uint32 param1, core::uint32 param2,
      core::uint32 value = 0);
  virtual void Finalize();

  virtual bool ExportConditionVar(core::uint32 &var, core::uint32 &value);

  virtual entity::TaskCondition::type GetType() const {
    return entity::TaskCondition::SOUL_APPEAR;
  }

  virtual bool CheckCondition();
  virtual void Synchronize();

  virtual void OnEvent(event::EventType::type type,
      const void *data, size_t size);

 private:
  void OnEventSoulAppear(const void *data, size_t size);

  // 要求幻化的次数
  core::uint32 require_count_;

  // 实际幻化的次数
  core::uint32 count_;
};

}  // namespace task

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TASK__SOUL__APPEAR__CONDITION__H

