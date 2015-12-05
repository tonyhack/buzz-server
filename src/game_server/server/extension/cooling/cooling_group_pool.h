//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-26 00:28:28.
// File name: cooling_group_pool.h
//
// Description:
// Define class CoolingGroupPool.
//

#ifndef __GAME__SERVER__COOLING__COOLING__GROUP__POOL__H
#define __GAME__SERVER__COOLING__COOLING__GROUP__POOL__H

#include "global/pool_template.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace cooling {

class CoolingGroup;

class CoolingGroupPool : public global::SingletonFactory<CoolingGroupPool> {
  friend class global::SingletonFactory<CoolingGroupPool>;

 public:
  bool Initialize(size_t initial_number, size_t extend_number);
  void Finalize();

  CoolingGroup *Allocate();
  void Deallocate(CoolingGroup *group);

 private:
  CoolingGroupPool() {}
  ~CoolingGroupPool() {}

  global::PoolTemplate<CoolingGroup> groups_;
};

}  // namespace cooling

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__COOLING__COOLING__GROUP__POOL__H

