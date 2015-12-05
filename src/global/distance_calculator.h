//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-16 18:11:26.
// File name: distance_calculator.h
//
// Description:
// Define class DistanceCalculator.
//

#ifndef __GLOBAL__DISTANCE__CALCULATOR__H
#define __GLOBAL__DISTANCE__CALCULATOR__H

#include <cmath>

#include "global/singleton_factory.h"

namespace global {

template<int kWidth = 128, int kHeight = 128>
class DistanceCalculatorTemplate :
  public global::SingletonFactory<DistanceCalculatorTemplate<kWidth, kHeight> > {
  friend class global::SingletonFactory<DistanceCalculatorTemplate<kWidth, kHeight> >;

 public:
  bool Initialize() {
    this->array_ = new (std::nothrow) core::uint32[(kWidth + 1) * (kHeight + 1)];
    if(this->array_ == NULL) {
      return false;
    }
    for(size_t i = 0; i < kHeight; ++i) {
      float y = pow(i, 2);
      for(size_t j = 0; j < kWidth; ++j) {
        float x = pow(j, 2);
        this->array_[i * (kWidth + 1) + j] = (core::uint32)pow(x + y, 0.5f);
      }
    }
    return true;
  }

  void Finalize() {
    delete [] this->array_;
  }

  core::uint32 Distance(core::uint32 src_x, core::uint32 src_y,
      core::uint32 dest_x, core::uint32 dest_y) {
    core::uint32 high = abs(src_y - dest_y);
    core::uint32 width = abs(src_x - dest_x);
    if(high > kHeight || width > kWidth)
      return core::kuint32max;
    else
      return this->array_[high * (kWidth + 1) + width];
  }

 private:
  DistanceCalculatorTemplate() {}
  ~DistanceCalculatorTemplate() {}

  core::uint32 *array_;
};

typedef DistanceCalculatorTemplate<2048, 2048> DistanceCalculator;

}  // namespace global

#endif  // __GLOBAL__DISTANCE__CALCULATOR__H

