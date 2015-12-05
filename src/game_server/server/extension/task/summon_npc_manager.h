//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2014-01-24 21:29:55.
// File name: summon_npc_manager.h
//
// Description:
//
#ifndef __GAME__SERVER__TASK__SUMMON__NPC__MANAGER__H
#define __GAME__SERVER__TASK__SUMMON__NPC__MANAGER__H

#include <map>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace task {

class SummonNpcManager : public core::Noncopyable {
 public:
  typedef std::pair<core::uint64, core::int32> Key;
  typedef std::map<Key, core::uint64> SceneNpcMap;

 public:
  SummonNpcManager();
  ~SummonNpcManager();

  bool Initialize();
  void Finalize();

  // 删除NPC
  bool RemoveNpc(core::int32 npc_template, core::uint64 scene_id);

  // 验证NPC template
  bool CheckNpc(core::int32 npc_template, core::uint64 scene_id);

  // 加入NPC
  bool AddNpc(core::int32 npc_template, core::uint64 scene_id,
      core::uint64 npc_id);
  
  // 得到NPC ID
  core::uint64 GetNpcID(core::int32 npc_template, core::uint64 scene_id) const {
    Key key(scene_id, npc_template);
    SceneNpcMap::const_iterator itr = this->npcs_.find(key);
    if(this->npcs_.end() == itr) {
      return 0;
    }
    return itr->second;
  }

 private:
  SceneNpcMap npcs_;
};

}  // namespace task

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TASK__SUMMON__NPC__MANAGER__H
