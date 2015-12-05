//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-02 22:27:54.
// File name: buff_pool.h
//
// Description:
// Define class BuffPool.
//

#ifndef __GAME__SERVER__BUFF__BUFF__POOL__H
#define __GAME__SERVER__BUFF__BUFF__POOL__H

#include "global/pool_template.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace buff {

class Buff;

class BuffPool : public global::SingletonFactory<BuffPool> {
  friend class global::SingletonFactory<BuffPool>;

 public:
  bool Initialize(size_t initial_number, size_t extend_number);
  void Finalize();

  Buff *Allocate();
  void Deallocate(Buff *buff);

 private:
  BuffPool() {}
  ~BuffPool() {}

  global::PoolTemplate<Buff> buffs_;
  core::uint64 index_;
};

}  // namespace buff

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__BUFF__BUFF__POOL__H

