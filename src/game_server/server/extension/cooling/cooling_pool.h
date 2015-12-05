//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-25 23:42:30.
// File name: cooling_pool.h
//
// Description:
// Define class CoolingPool.
//

#ifndef __GAME__SERVER__COOLING__COOLING__POOL__H
#define __GAME__SERVER__COOLING__COOLING__POOL__H

#include "global/pool_template.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace cooling {

class Cooling;

class CoolingPool : public global::SingletonFactory<CoolingPool> {
  friend class global::SingletonFactory<CoolingPool>;

 public:
  bool Initialize(size_t initial_number, size_t extend_number);
  void Finalize();

  Cooling *Allocate();
  void Deallocate(Cooling *cooling);

 private:
  CoolingPool() {}
  ~CoolingPool() {}

  global::PoolTemplate<Cooling> coolings_;
};

}  // namespace cooling

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__COOLING__COOLING__POOL__H

