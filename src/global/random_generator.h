//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-06 17:21:54.
// File name: random_generator.h
//
// Description:
// Define class RandomGenerator.
//

#ifndef __GLOBAL__RANDOM__GENERATOR__h
#define __GLOBAL__RANDOM__GENERATOR__h

#include <stdlib.h>
#include <time.h>
#include <cmath>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "global/logging.h"

namespace global {

class RandomGenerator : public core::Noncopyable {
 public:
  RandomGenerator() : random_number_(0), seed_(0) {}
  ~RandomGenerator() {}

  void Initialize() {
    this->Seed();
  }

  void Seed(core::uint32 seed) {
    this->seed_ = seed;
    srand(this->seed_);
  }

  core::uint32 Seed() {
    this->random_number_ = 0;
    this->Seed((unsigned int)::time(NULL));
    return this->seed_;
  }

  // [0, RAND_MAX]
  core::uint32 Random() {
    if(++this->random_number_ > kMaxRandomTime_) {
      this->Seed();
    }
    return ::rand();
  }

  // [0, max)
  core::uint32 Random(core::uint32 max) {
    double d = this->Random() / ((double)RAND_MAX + 1);
    return (core::uint32)(d * max);
  }

  // [min, max]
  core::uint32 Uniform(core::uint32 min, core::uint32 max) {
    double d = this->Random() / (double)RAND_MAX;
    return (core::uint32)::round((min + (max - min) * d));
  }

 private:
  core::uint32 random_number_;
  core::uint32 seed_;

  static const core::uint32 kMaxRandomTime_ = 99999;
};

}  // namespace global

#endif  // __GLOBAL__RANDOM__GENERATOR__h

