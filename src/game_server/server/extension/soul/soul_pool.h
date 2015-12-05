//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-17 11:08:50.
// File name: soul_pool.h
//
// Description:
// Define class SoulPool.
//

#ifndef __GAME__SERVER__SOUL__SOUL__POOL__H
#define __GAME__SERVER__SOUL__SOUL__POOL__H

#include "global/pool_template.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace soul {

class Soul;

class SoulPool : public global::SingletonFactory<SoulPool> {
  friend class global::SingletonFactory<SoulPool>;

 public:
  bool Initialize(size_t initial_number, size_t extend_number);
  void Finalize();

  Soul *Allocate();
  void Deallocate(Soul *soul);

 private:
  SoulPool() {}
  ~SoulPool() {}

  global::PoolTemplate<Soul> souls_;
};

}  // namespace soul

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SOUL__SOUL__POOL__H

