#ifndef __GAME__SERVER__WELFARE__CONDITION__H
#define __GAME__SERVER__WELFARE__CONDITION__H

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "entity/achieve_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension/welfare/achieve_imp_event.h"

namespace game {

namespace server {

namespace welfare {

class Task;

class Condition : public core::Noncopyable {
 public:
 friend class AchieveImpEvent;

  Condition();
  virtual ~Condition();

  bool Initialize(Task *task, entity::AchieveConditionType::type type, core::uint32 param1, core::uint32 param2,
      core::uint32 param3, core::uint32 value = 0);
  void Finalize();

  // 导出条件变量
  bool ExportConditionVar(core::int32 param1, core::int32 &param2, core::int32 &value);

  entity::AchieveConditionType::type GetType() {
    return this->type_;
  }

  core::int32 GetValue() {
    return (core::int32)this->value_;
  }

  void Clear();

  // 条件检测
  bool CheckCondition();

  // 得到任务对象
  Task *GetTask();

  // 事件回调
  void OnEvent(event::EventType::type type,
      const void *data, size_t size);

 private:
  Task *task_;
  AchieveImpEvent imp_;
  entity::AchieveConditionType::type type_;

  core::uint32 param1_;
  core::uint32 param2_;
  core::uint32 param3_;
  core::uint32 value_;
};

}  // namespace welfare

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__WELFARE__CONDITION__H
