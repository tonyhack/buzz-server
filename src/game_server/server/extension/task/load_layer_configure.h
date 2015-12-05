//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-12-25 21:16:39.
// File name: load_layer_configure.h
//
// Description:
//
#ifndef __GAME__SERVER__TASK__LOAD__LAYER__CONFIGURE__H
#define __GAME__SERVER__TASK__LOAD__LAYER__CONFIGURE__H

#include <map>
#include <vector>
#include <ext/hash_map>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "game_server/server/extension/task/task_configure.h"

namespace game {

namespace server {

namespace task {

class TaskLayerCell {
  public:
   core::int32 layer_;
   core::uint32 task_id_;
   core::int32 physique_add_value_;
   core::int32 strength_add_value_;
   core::int32 intelligence_add_value_;
   core::int32 agile_add_value_;
   core::int32 spirit_add_value_;
};

class LoadLayerConfigure : public core::Noncopyable {
 typedef std::map<core::int32, TaskLayerCell> TaskLayerMap;
 typedef TaskLayerMap::iterator TaskLayerMapItr;
 public:
  LoadLayerConfigure();
  ~LoadLayerConfigure();

  bool LoadConfigure(const std::string &file);

  const TaskLayerCell *GetCell(core::uint32 task_id) const;

 private:
  TaskLayerMap layers_;
};

}  // namespace task

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TASK__LOAD__LAYER__CONFIGURE__H

