//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-08 15:49:44.
// File name: grid_map.h
//
// Description:
// Define class GridMap.
//

#ifndef __GAME__SERVER__SCENE__GRID__MAP__H
#define __GAME__SERVER__SCENE__GRID__MAP__H

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace scene {

class Grid;

class GridMap : public core::Noncopyable {
 public:
  GridMap();
  ~GridMap();

  bool Allocate(core::uint32 max_x, core::uint32 max_y);
  void Deallocate();

  bool Initialize();
  void Finalize();

  Grid *Get(core::uint32 x, core::uint32 y);
  const Grid *Get(core::uint32 x, core::uint32 y) const;

  // 格子表
  Grid *grids_;

  // 格子表长度
  core::uint32 size_;

  // 最大x和y
  core::uint32 max_x_;
  core::uint32 max_y_;
};

}  // namespace scene

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SCENE__GRID__MAP__H

