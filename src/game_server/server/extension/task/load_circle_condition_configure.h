//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-10-15 02:05:38.
// File name: load_circle_condition_configure.h
//
// Description:
//
#ifndef __GAME__SERVER__TASK__LOAD__CIRCLE__CONDITION__CONFIGURE__H
#define __GAME__SERVER__TASK__LOAD__CIRCLE__CONDITION__CONFIGURE__H

#include <map>
#include <vector>
#include <ext/hash_map>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "game_server/server/extension/task/task_configure.h"

namespace game {

namespace server {

namespace task {

class ConditionCell {
 public:
  TaskCondition condition_;
  core::int32 star_;
  core::int32 order_id_;
  core::int32 id_;
  TaskCoordinate coordinate_;
};

class LoadCircleConditionConfigure : public core::Noncopyable {
  typedef std::vector<ConditionCell> ConditionVec;
  typedef std::map<core::int32, ConditionVec> ConditionRandMap;
  typedef std::map<core::int32, ConditionCell> ConditionMap;

 public:
  LoadCircleConditionConfigure();
  ~LoadCircleConditionConfigure();

  bool LoadConfigure(const std::string &file);

  const ConditionCell *RandCondition(core::int32 lv_stage) const;
  
  const ConditionCell *GetCondition(core::int32 id) const;

 private:
  ConditionMap task_conditions_;
  ConditionRandMap task_rand_conditions_;
};

}  // namespace task

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TASK__LOAD__CIRCLE__CONDITION__CONFIGURE__H

