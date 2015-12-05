//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2014-01-04 20:50:33.
// File name: load_world_boss_configure.h
//
// Description:
//
#ifndef __GLOBAL__LOAD__WORLD__BOSS__CONFIGURE__H
#define __GLOBAL__LOAD__WORLD__BOSS__CONFIGURE__H

#include <string>
#include <vector>
#include <ext/hash_map>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace global {

namespace configure {

class BossCoordinateCell {
 public:
  core::int32 x_;                     // x
  core::int32 y_;                     // y
  core::int32 width_;                 // 宽
  core::int32 heigth_;                // 高
};

class WorldBossCell {
 public:

  WorldBossCell() {}
  ~WorldBossCell() {}

  core::uint32 template_id_;          // npc id
  core::int32 map_id_;                // 地图id
  BossCoordinateCell coordinates_[4]; // 世界BOSS刷新点
};

class LoadWorldBossConfigure : public core::Noncopyable {
 public:
  typedef __gnu_cxx::hash_map<core::uint32, WorldBossCell> NpcHashMap;

  LoadWorldBossConfigure();
  ~LoadWorldBossConfigure();

  bool LoadConfigure(const std::string &file);

  const WorldBossCell *GetCell(core::int32 map_id) const;

 private:
  NpcHashMap npcs_;
};

}  // namespace global

}  // namespace configure

#endif  // __GLOBAL__LOAD__WORLD__BOSS__CONFIGURE__H

