//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-10-14 23:50:36.
// File name: load_task_circle_configure.h
//
// Description:
//

#ifndef __GAME__SERVER__TASK__LOAD__TASK__CIRCLE__CONFIGURE__H
#define __GAME__SERVER__TASK__LOAD__TASK__CIRCLE__CONFIGURE__H

#include <vector>
#include <map>
#include <string>

#include "game_server/server/extension/task/task_configure.h"

namespace game {

namespace server {

namespace task {

class TaskCircleCell {
 public:
  core::int32 refresh_condition_;
  core::int32 jump_spend_gold_;
  core::int32 star_coe_;
  std::vector<TaskAward> awards_;
};

class TaskCircleKey {
 public:
  core::int32 id_;
  core::int32 circle_num_;

  bool operator< (const TaskCircleKey& key) const {
    if(this->circle_num_ != key.circle_num_) {
      return this->circle_num_ < key.circle_num_;
    } else {
      return this->id_ < key.id_;
    }
  }
};

class LoadTaskCircleConfigure {
  typedef std::map<TaskCircleKey, TaskCircleCell> CircleMap;

 public:
  LoadTaskCircleConfigure();
  ~LoadTaskCircleConfigure();

  bool LoadConfigure(const std::string &file);

  const TaskCircleCell* GetTaskCircleCell(const TaskCircleKey& key);

 private:
   CircleMap task_circles_;
};

}  // namespace task

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TASK__LOAD__TASK__CIRCLE__CONFIGURE__H

