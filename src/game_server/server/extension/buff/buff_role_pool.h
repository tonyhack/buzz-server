//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-04 00:24:40.
// File name: buff_role_pool.h
//
// Description:
// Define class BuffRolePool.
//

#ifndef __GAME__SERVER__BUFF__BUFF__ROLE__POOL__h
#define __GAME__SERVER__BUFF__BUFF__ROLE__POOL__h

#include "global/pool_template.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace buff {

class BuffRole;

class BuffRolePool : public global::SingletonFactory<BuffRolePool> {
  friend class global::SingletonFactory<BuffRolePool>;

 public:
  bool Initialize(size_t initial_number, size_t extend_number);
  void Finalize();

  BuffRole *Allocate();
  void Deallocate(BuffRole *role);

 private:
  BuffRolePool() {}
  ~BuffRolePool() {}

  global::PoolTemplate<BuffRole> roles_;
};

}  // namespace buff

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__BUFF__BUFF__ROLE__POOL__h

