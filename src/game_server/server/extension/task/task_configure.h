//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-01 17:40:20.
// File name: task_configure.h
//
// Description:
// Define class TaskConfigure.
//

#ifndef __GAME__SERVER__TASK__TASK__CONFIGURE__H
#define __GAME__SERVER__TASK__TASK__CONFIGURE__H

#include <vector>

#include "core/base/types.h"

namespace game {

namespace server {

namespace task {

class TaskCondition {
 public:
  TaskCondition() : type_(0), param1_(0), param2_(0) {}

  // 类型
  core::uint32 type_;

  // 参数
  core::int32 param1_;
  core::int32 param2_;
};

class TaskCoordinate {
 public:
  TaskCoordinate() : x_(0), y_(0), map_(0) {}

  core::int32 x_;
  core::int32 y_;
  core::int32 map_;
};

class TaskAward {
 public:
  TaskAward() : type_(0), id_(0), number_(0), intensify_level_(0) {}

  // 类型
  core::uint32 type_;

  // ID/数量
  core::uint32 id_;
  core::uint32 number_;

  // 是否绑定(0:非绑, 1:绑定)
  core::uint32 bind_;

  // 如果是装备，装备的强化等级
  core::int32 intensify_level_;
};

// 杀怪收集配置
class KillCollectCell {
 public:
  // NPC的templateid
  core::uint32 npc_;

  // 任务ID
  core::int32 task_id_;

  // 收集成功的机率
  core::uint32 odds_;

  // 收集的道具ID
  core::int32 template_id_;
};
/*
class TaskAwardItem {
 public:
  TaskAwardItem() : id_(0), bind_(0) {}

  // 道具ID
  core::uint32 id_;
  // 绑定类型(0:非绑, 1:绑定)
  core::uint32 bind_;
};
*/
class TaskCell {
 public:
  typedef std::vector<TaskCoordinate> CoordinateVec;

  TaskCell() {}

  // 接受/完成 NPCID
  core::uint32 accept_npc_;
  core::uint32 finish_npc_;

  // 接受任务NPC
  TaskCoordinate accept_coordinate_;
  TaskCoordinate finish_coordinate_;
  CoordinateVec condition_coordinates_;

  // 接受最小等级
  core::int32 min_level_;

  // 前置/后置 任务ID
  core::uint32 pre_task_id_;
  core::uint32 next_task_id_;

  // 倒计时(秒数)
  core::uint32 time_;

  // 是否可放弃(0:不可以 1:可以)
  core::uint32 can_giveup_;

  // 任务完成条件
  TaskCondition condition_[3];

  // 任务奖励
  TaskAward award_[5];

  // 增加道具奖励(五选一)
  TaskAward award_item_[5];
};

class TaskTransportCell {
 public:
  typedef std::vector<TaskAward> AwardVector;

  core::int32 id_;
  core::int32 npc_id_;
  core::int32 odds_;

  AwardVector success_awards_;
  AwardVector rob_awards_;
  AwardVector failure_awards_;

  core::int32 force_success_spend_;
};

}  // namespace task

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TASK__TASK__CONFIGURE__H

