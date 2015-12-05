#ifndef __GAME__SERVER__TASK__KILL__MOB__TYPE__COLLECT__CONDITION__H
#define __GAME__SERVER__TASK__KILL__MOB__TYPE__COLLECT__CONDITION__H

#include "core/base/types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension/task/condition.h"

namespace game {

namespace server {

namespace task {

class KillMobTypeCondition : public Condition {
 public:
  KillMobTypeCondition();
  virtual ~KillMobTypeCondition();

  virtual bool Initialize(Task *task, core::uint32 param1, core::uint32 param2,
      core::uint32 value = 0);
  virtual void Finalize();

  virtual bool ExportConditionVar(core::uint32 &var, core::uint32 &value);

  virtual entity::TaskCondition::type GetType() const {
    return entity::TaskCondition::KILL_MOB_TYPE;
  }

  virtual bool CheckCondition();
  virtual void Synchronize();

  virtual void OnEvent(event::EventType::type type,
      const void *data, size_t size);
  
  virtual void Clear();

 private:
  void OnEvent(const void *data, size_t size);

  core::int32 module_type_;
  // 需要道具的数目
  core::uint32 quired_num_;

  core::uint32 count_;
};

}  // namespace task

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TASK__KILL__MOB__TYPE__COLLECT__CONDITION__H

