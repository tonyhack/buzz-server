//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-13 19:25:46.
// File name: jump_configure.h
//
// Description:
// Define class JumpConfigure.
//

#ifndef __GAME__SERVER__SCENE__JUMP__CONFIGURE__H
#define __GAME__SERVER__SCENE__JUMP__CONFIGURE__H

#include <ext/hash_map>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace scene {

class JumpCell {
 public:
  JumpCell() {}
  ~JumpCell() {}

  core::uint32 dest_map_;         // 目标地图ID
  core::uint32 dest_x_;           // 目标地图坐标x
  core::uint32 dest_y_;           // 目标地图坐标y
};

class JumpConfigure : public core::Noncopyable {
  typedef __gnu_cxx::hash_map<core::uint32, JumpCell> JumpHashmap;

 public:
  JumpConfigure();
  ~JumpConfigure();

  bool LoadConfigure(const std::string &file);

  const JumpCell *GetJump(core::uint32 id) const;

 private:
  JumpHashmap jumps_;
};

}  // namespace scene

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SCENE__JUMP__CONFIGURE__H

