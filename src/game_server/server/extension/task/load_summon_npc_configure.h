//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2014-01-24 01:01:28.
// File name: load_summon_npc_configure.h
//
// Description:
//
#ifndef __GAME__SERVER__TASK__LOAD__SUMMON__NPC__CONFIGURE__H
#define __GAME__SERVER__TASK__LOAD__SUMMON__NPC__CONFIGURE__H

#include <map>
#include <vector>
#include <ext/hash_map>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "game_server/server/extension/task/task_configure.h"

namespace game {

namespace server {

namespace task {

class SummonNpcCell {
 public:
  core::int32 id_;          // 被召唤的NPC ID       
  core::int32 x_;           // 坐标 x;
  core::int32 y_;
  core::int32 npc_id_;      // 召唤NPC id;
};

class LoadSummonNpcConfigure : public core::Noncopyable {
  typedef std::map<core::int32, SummonNpcCell> SummonNpcMap;

 public:
  LoadSummonNpcConfigure();
  ~LoadSummonNpcConfigure();

  bool LoadConfigure(const std::string &file);

  const SummonNpcCell *GetCell(core::int32 id) const;

 private:
  SummonNpcMap npcs_;

};

}  // namespace task

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TASK__LOAD__SUMMON__NPC__CONFIGURE__H

