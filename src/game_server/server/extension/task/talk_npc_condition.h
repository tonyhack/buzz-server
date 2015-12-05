//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-16 10:40:15.
// File name: talk_npc_condition.h
//
// Description:
// Define class TalkNpcCondition.
//

#ifndef __GAME__SERVER__TASK__TALK__NPC__CONDITION__H
#define __GAME__SERVER__TASK__TALK__NPC__CONDITION__H

#include "core/base/types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension/task/condition.h"

namespace game {

namespace server {

namespace task {

class TalkNpcCondition : public Condition {
 public:
  TalkNpcCondition();
  virtual ~TalkNpcCondition();

  virtual bool Initialize(Task *task, core::uint32 param1, core::uint32 param2,
      core::uint32 value = 0);
  virtual void Finalize();

  // 导出条件变量
  virtual bool ExportConditionVar(core::uint32 &var, core::uint32 &value);

  virtual entity::TaskCondition::type GetType() const {
    return entity::TaskCondition::TALK;
  }

  virtual bool CheckCondition();
  virtual void Synchronize();

  virtual void OnEvent(event::EventType::type type,
      const void *data, size_t size);

 private:
  // 对话NPC回调
  void OnEventTalkNpc(const void *data, size_t size);

  // NPC模板ID
  core::uint32 npc_;

  // 要求对话次数
//  core::uint32 require_talk_;

  // 对话次数
  core::uint32 talk_; //0 : 没有对话 ， 1 ：对话完成
};

}  // namespace task

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TASK__TALK__NPC__CONDITION__H

