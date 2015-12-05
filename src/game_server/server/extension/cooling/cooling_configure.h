//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-02 17:04:26.
// File name: cooling_configure.h
//
// Description:
// Define class CoolingConfigure.
//

#ifndef __GAME__SERVER__COOLING__COOLING__CONFIGURE__H
#define __GAME__SERVER__COOLING__COOLING__CONFIGURE__H

namespace game {

namespace server {

namespace cooling {

class CoolingCell {
 public:
  CoolingCell() {}

  // 冷却组ID
  core::uint32 group_;
  // 冷却时间(毫秒)
  core::uint32 time_;
};

class GroupCell {
 public:
  GroupCell() {}

  // 组冷却时间
  core::uint32 time_;
};

}  // namespace cooling

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__COOLING__COOLING__CONFIGURE__H

