//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-01-27 17:58:05.
// File name: direction_calculator.h
//
// Description:
// Define class DirectionCalculator.
//

#ifndef __GLOBAL__DIRECTION__CALCULATOR__H
#define __GLOBAL__DIRECTION__CALCULATOR__H

#include <cmath>

#include <map>
#include <utility>

#include "core/base/types.h"
#include "entity/direction_types.h"
#include "global/singleton_factory.h"

namespace global {

template<core::int32 kWidth = 32, core::int32 kHeight = 32>
class DirectionCalculatorTemplate :
  public global::SingletonFactory<DirectionCalculatorTemplate<kWidth, kHeight> > {
  friend class global::SingletonFactory<DirectionCalculatorTemplate<kWidth, kHeight> >;

  typedef std::pair<core::int32, core::int32> Key;
  typedef std::map<Key, core::int32> DirectionMap;

 public:
  bool Initialize() {
    for(core::int32 i = -kWidth; i < kWidth; ++i) {
      for(core::int32 j = -kHeight; j < kHeight; ++j) {
        this->directions_.insert(std::make_pair(Key(i, j),
              core::int32(atan2(j, i) * 180 / kPI_)));
      }
    }
    return true;
  }

  void Finalize() {
    this->directions_.clear();
  }

  entity::DirectionType::type GetGridDirection(core::int32 src_x, core::int32 src_y,
      core::int32 dest_x, core::int32 dest_y) const {
    return this->ConvertGridDirection(this->Direction(src_x, src_y, dest_x, dest_y));
  }

  core::int32 Direction(core::int32 src_x, core::int32 src_y,
      core::int32 dest_x, core::int32 dest_y) const {
    Key key(dest_x - src_x, src_y - dest_y);
    DirectionMap::const_iterator iterator = this->directions_.find(key);
    if(iterator != this->directions_.end()) {
      return iterator->second;
    } else {
      return core::kint32max;
    }
  }

  entity::DirectionType::type ConvertGridDirection(core::int32 direction) const {
    if(direction >= -22 && direction <= 22) {
      return entity::DirectionType::EAST;
    } else if(direction >= 23 && direction <= 67) {
      return entity::DirectionType::NORTHEAST;
    } else if(direction >= 68 && direction <= 112) {
      return entity::DirectionType::NORTH;
    } else if(direction >= 113 && direction <= 157) {
      return entity::DirectionType::NORTHWEST;
    } else if((direction >= 158 && direction <= 180) || (direction >= -180 && direction <= -158)) {
      return entity::DirectionType::WEST;
    } else if(direction >= -67 && direction <= -22) {
      return entity::DirectionType::SOUTHEAST;
    } else if(direction >= -112 && direction <= -68) {
      return entity::DirectionType::SOUTH;
    } else if(direction >= -157 && direction <= -113) {
      return entity::DirectionType::SOUTHWEST;
    } else {
      return entity::DirectionType::NONE;
    }
  }

 private:
  DirectionCalculatorTemplate() {}
  ~DirectionCalculatorTemplate() {}

  DirectionMap directions_;

  static const double kPI_ = 3.14159;
};

typedef DirectionCalculatorTemplate<32, 32> DirectionCalculator;

}  // namespace global

#endif  // __GLOBAL__DIRECTION__CALCULATOR__H

