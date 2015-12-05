//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-10-24 01:37:02.
// File name: load_trip_configure.h
//
// Description:
//
#ifndef __GAME__SERVER__TASK__LOAD__TRIP__CONFIGURE__H
#define __GAME__SERVER__TASK__LOAD__TRIP__CONFIGURE__H

#include <map>
#include <vector>
#include <ext/hash_map>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "game_server/server/extension/task/task_configure.h"

namespace game {

namespace server {

namespace task {

class TripTaskKey {
 public:
  core::int32 id_;
  core::int32 type_;

  bool operator <(const TripTaskKey& key) const {
    if(this->id_ != key.id_) {
      return this->id_ <  key.id_;
    } else {
      return this->type_ < key.type_;
    }
  }
};

class TripTaskCell {
 public:
  core::int32 id_;
  core::int32 type_;
  core::int32 order_id_;
  std::vector<TaskAward> awards_;
  core::int32 spent_;
  TaskCondition condition_;
  TaskCoordinate coordinate_;
};

enum TripTaskChildType {
  type_1 = 1,
  type_2 = 2,
  type_3 = 3,
};

class LoadTripConfigure : public core::Noncopyable {
  typedef std::map<core::int32, TripTaskCell> ConditionMap;

 public:
  typedef std::vector<TripTaskCell> RandConditionVec;
  typedef std::map<TripTaskKey, RandConditionVec> ConditionRandMap;

  LoadTripConfigure();
  ~LoadTripConfigure();

  bool LoadConfigure(const std::string &file);

  const RandConditionVec *GetRandCondition(core::int32 stage, core::int32 type) const;
  
  const TripTaskCell *GetCondition(core::int32 id) const;

 private:
  ConditionMap task_conditions_;
  ConditionRandMap task_rand_conditions_;
};

}  // namespace task

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TASK__LOAD__TRIP__CONFIGURE__H

