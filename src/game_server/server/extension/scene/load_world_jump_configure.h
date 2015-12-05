//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-12-14 02:40:00.
// File name: load_world_jump_configure.h
//
// Description:
//

#ifndef __GAME__SERVER__SCENE__LOAD__WORLD__JUMP__CONFIGURE__H
#define __GAME__SERVER__SCENE__LOAD__WORLD__JUMP__CONFIGURE__H

#include <ext/hash_map>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace scene {

class WorldJumpCell {
 public:
  core::uint32 map_id_;
  core::int32 x_;
  core::int32 y_;
};

class LoadWorldJumpConfigure : public core::Noncopyable {
  typedef __gnu_cxx::hash_map<core::uint32, WorldJumpCell> JumpHashmap;

 public:
  LoadWorldJumpConfigure();
  ~LoadWorldJumpConfigure();

  bool LoadConfigure(const std::string &file);

  const WorldJumpCell *GetJumpCell(core::uint32 map_id) const;

 private:  
  JumpHashmap jumps_;
};

}  // namespace scene

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SCENE__LOAD__WORLD__JUMP__CONFIGURE__H

