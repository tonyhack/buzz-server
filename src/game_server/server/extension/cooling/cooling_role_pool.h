//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-26 18:25:50.
// File name: cooling_role_pool.h
//
// Description:
// Define class CoolingRolePool.
//

#ifndef __GAME__SERVER__COOLING__COOLING__ROLE__POOL__H
#define __GAME__SERVER__COOLING__COOLING__ROLE__POOL__H

#include "global/pool_template.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace cooling {

class CoolingRole;

class CoolingRolePool : public global::SingletonFactory<CoolingRolePool> {
  friend class global::SingletonFactory<CoolingRolePool>;

 public:
  bool Initialize(size_t initial_number, size_t extend_number);
  void Finalize();

  CoolingRole *Allocate();
  void Deallocate(CoolingRole *role);

 private:
  CoolingRolePool() {}
  ~CoolingRolePool() {}

  global::PoolTemplate<CoolingRole> roles_;
};

}  // namespace cooling

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__COOLING__COOLING__ROLE__POOL__H

