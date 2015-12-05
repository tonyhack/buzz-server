//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-10 17:09:59.
// File name: facade_request.h
//
// Description:
// Define class FacadeRequest.
//

#ifndef __GAME__SERVER__TASK__FACADE__REQUEST__H
#define __GAME__SERVER__TASK__FACADE__REQUEST__H

#include "core/base/types.h"
#include "game_server/server/extension/task/load_circle_condition_configure.h"
#include "game_server/server/extension/task/load_task_circle_configure.h"
#include "game_server/server/extension/task/task_configure.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace task {

struct BindType {
  enum type {
    NOT_BIND = 0,
    BIND = 1,
    BOTH = 2,
  };
};

class FacadeRequest : public global::SingletonFactory<FacadeRequest> {
  friend class global::SingletonFactory<FacadeRequest>;

 public:
  // 增加道具到背包
  bool AddItem(core::uint64 actor, core::uint32 item_template,
      core::uint32 number, bool bind = false);

  // 物品是否存在于背包
  bool CheckPacketItemExist(core::uint64 actor, core::uint32 item_template,
      core::uint32 number, BindType::type bind);
  // 背包删除物品
  core::uint32 RemovePacketItem(core::uint64 actor, core::uint32 item_template,
      core::uint32 number, BindType::type bind);

  // 添加道具并且检查
  bool Awards(core::uint64 actor, const TaskAward *awards, core::int32 num);

  bool Awards(core::uint64 actor, const std::vector<TaskAward> &awards);

  // 添加循环任务方面的奖励 round(是不是循环任务的轮奖励)
  bool AwardsCircleTask(core::uint64 actor, const TaskCircleCell *cell, 
      const ConditionCell * conditionCell, bool round, core::int32 number);

  // 护送是否在周围
  bool CheckTransportAround(core::uint64 actor);

  // 查询当前护送NPC
  core::uint64 RequestCurrentTransportNpc(core::uint64 actor);

  // 护送
  core::uint64 RequestTransport(core::uint64 actor, core::int32 npc_template);

  // 清除护送
  bool RemoveTransport(core::uint64 actor);

 private:
  FacadeRequest() {}
  ~FacadeRequest() {}
};

}  // namespace task

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TASK__FACADE__REQUEST__H

