//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-05 16:08:25.
// File name: configure.h
//
// Description:
// Define class Configure.
//

#ifndef __GAME__SERVER__TASK__CONFIGURE__H
#define __GAME__SERVER__TASK__CONFIGURE__H

#include <string>

#include "core/base/types.h"
#include "game_server/server/extension/task/configure.h"
#include "game_server/server/extension/task/task_configure.h"
#include "game_server/server/extension/task/load_circle_condition_configure.h"
#include "game_server/server/extension/task/load_exploit_attr_configure.h"
#include "game_server/server/extension/task/load_kill_collect_configure.h"
#include "game_server/server/extension/task/load_layer_configure.h"
#include "game_server/server/extension/task/load_summon_npc_configure.h"
#include "game_server/server/extension/task/load_task_circle_configure.h"
#include "game_server/server/extension/task/load_task_configure.h"
#include "game_server/server/extension/task/load_trip_configure.h"
#include "game_server/server/extension/task/load_task_daily_configure.h"
#include "game_server/server/extension/task/load_task_exploit_award_configure.h"
#include "game_server/server/extension/task/load_task_playing.h"
#include "game_server/server/extension/task/load_task_transport_configure.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace task {

class Configure : public global::SingletonFactory<Configure> {
  friend class global::SingletonFactory<Configure>;

 public:
  bool LoadConfigure(const std::string &file);

  // 获取任务配置
  const TaskCell *GetTaskConfigure(core::uint32 id);

  // 获取杀怪收集配置
  const KillCollectCell *GetKillCollect(const core::uint32 key);

  // 获取循环任务配置
  const TaskCircleCell *GetCircleCell(core::int32 lv_stage, core::int32 circle_num);

  // 获取循环任务的任务条件
  const ConditionCell *GetConditionCell(core::int32 id) const;

  // 随机等级段内的循环任务条件
  const ConditionCell *RandConditionCell(core::int32 lv_stage) const;

  // 获取等级段指定种类的奇遇任务配置
  const LoadTripConfigure::RandConditionVec *GetRandCondition(core::int32 stage, core::int32 type) const;

  // 获取奇遇任务配置
  const TripTaskCell *GetTripConditionCell(core::int32 id) const;

  // 获取任务纹章配置
  const TaskLayerCell *GetLayerCell(core::uint32 task_id) const;

  // 获取召唤NPC任务配置
  const SummonNpcCell *GetSummonNpcCell(core::uint32 npc_template) const;

  // 检查副本任务条件
  bool CheckPlayingCondition(core::int32 id, core::int32 map_id) const;

  // 获取每日任务配置
  const TaskDailyCell *GetTaskDailyCell(core::int32 id) const;

  const LoadTaskDailyConfigure::TaskDailyMap &GetDailyList() const {
    return this->load_daily_.GetDailyList();
  }

  // 获取每日奖励军功
  const ExploitAwardCell *GetTaskDailyAwardCell(core::int32 id) const;

  const LoadTaskExploitAwardConfigure::AwardMap &GetExploitAwardConfigure() const {
    return this->load_exploit_award_.GetConfig();
  }

  // 获取军功属性加成
  const ExploitAttrCell *GetExploitAttrCell(core::int32 exploit) const {
    return this->exploit_attr_.GetCell(exploit);
  }

  const LoadTaskTransportConfigure &GetTransportConfig() const {
    return this->transport_;
  }

  // Condition对象池
  int condition_initial_number_;
  int condition_extend_number_;

  // Task对象池
  int task_initial_number_;
  int task_extend_number_;

  // 与NPC的对话距离
  static const core::uint32 kTalkDistance_;

 private:
  Configure() {}
  ~Configure() {}

  LoadKillCollectConfigure load_kill_collect_;
  LoadTaskConfigure load_task_;
  LoadTaskCircleConfigure load_circle_; 
  LoadCircleConditionConfigure load_circle_condition_;
  LoadTripConfigure load_trip_;
  LoadLayerConfigure load_layer_;
  LoadSummonNpcConfigure load_summon_npc_;
  LoadTaskDailyConfigure load_daily_;
  LoadTaskExploitAwardConfigure load_exploit_award_;
  LoadTaskPlayingConfigure load_playing_;
  LoadExploitAttrConfigure exploit_attr_;
  LoadTaskTransportConfigure transport_;
};

}  // namespace task

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TASK__CONFIGURE__H

