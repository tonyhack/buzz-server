//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-08 15:58:42.
// File name: grid_map.cc
//
// Description:
// Define class class GridMap.
//

#include "game_server/server/extension/scene/grid_map.h"

#include <new>
#include <stddef.h>

#include "game_server/server/extension/scene/grid.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace scene {

GridMap::GridMap() : grids_(NULL), size_(0), max_x_(0), max_y_(0) {}
GridMap::~GridMap() {}

bool GridMap::Allocate(core::uint32 max_x, core::uint32 max_y) {
  this->size_ = max_x * max_y;
  this->grids_ = new (std::nothrow) Grid[this->size_];
  if(this->grids_ == NULL) {
    global::LogError("%s:%d (%s) Allocate girds[size=%lu] failed.",
        __FILE__, __LINE__, __FUNCTION__, this->size_);
    return false;
  }
  this->max_x_ = max_x;
  this->max_y_ = max_y;

  return true;
}

void GridMap::Deallocate() {
  if(this->grids_) {
    delete [] this->grids_;
  }
}

bool GridMap::Initialize() {
  size_t pos = 0;
  for(; pos < this->size_; ++pos) {
    if(this->grids_[pos].Initialize() == false) {
      global::LogError("%s:%d (%s) Initialize gird failed.",
          __FILE__, __LINE__, __FUNCTION__);
      return false;
    }
  }

  return true;
}

void GridMap::Finalize() {
  size_t pos = 0;
  for(; pos < this->size_; ++pos) {
    this->grids_[pos].Finalize();
  }
}

Grid *GridMap::Get(core::uint32 x, core::uint32 y) {
  core::uint32 index = y * this->max_x_ + x;
  if(index >= this->size_) {
    // global::LogError("%s:%d (%s) Error index[%d,%d].",
    //     __FILE__, __LINE__, __FUNCTION__, x, y);
    return NULL;
  }
  return &this->grids_[index];
}

const Grid *GridMap::Get(core::uint32 x, core::uint32 y) const {
  core::uint32 index = y * this->max_x_ + x;
  if(index >= this->size_) {
    // global::LogError("%s:%d (%s) Error index[%d,%d].",
    //     __FILE__, __LINE__, __FUNCTION__, x, y);
    return NULL;
  }
  return &this->grids_[index];
}

}  // namespace scene

}  // namespace server

}  // namespace game

