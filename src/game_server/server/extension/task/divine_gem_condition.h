//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-10-11 00:42:42.
// File name: divine_gem_condition.h
//
// Description:
//
#ifndef __GAME__SERVER__TASK__DIVINE__GEM__CONDITION__H
#define __GAME__SERVER__TASK__DIVINE__GEM__CONDITION__H

#include "core/base/types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension/task/condition.h"

namespace game {

namespace server {

namespace task {

class DivineGemCondition : public Condition {
 public:
  DivineGemCondition();
  virtual ~DivineGemCondition();

  virtual bool Initialize(Task *task, core::uint32 param1, core::uint32 param2,
      core::uint32 value = 0);
  virtual void Finalize();

  // 导出条件变量
  virtual bool ExportConditionVar(core::uint32 &var, core::uint32 &value);

  virtual entity::TaskCondition::type GetType() const {
    return entity::TaskCondition::DIVINE_GEM;
  }

  virtual bool CheckCondition();
  virtual void Synchronize();

  virtual void OnEvent(event::EventType::type type,
      const void *data, size_t size);

 private:
  // 杀死怪物回调
  void OnEventDivineGem(const void *data, size_t size);

  // 占星当前次数
  core::uint32 count_;

  // 英灵需要次数
  core::uint32 quired_num_;
};

}  // namespace task

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TASK__DIVINE__GEM__CONDITION__H

