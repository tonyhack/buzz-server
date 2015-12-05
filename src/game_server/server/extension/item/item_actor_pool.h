//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-20 10:18:33.
// File name: item_actor_pool.h
//
// Description:
// Define class ItemActorPool.
//

#ifndef __GAME__SERVER__ITEM__ITEM__ACTOR__POOL__H
#define __GAME__SERVER__ITEM__ITEM__ACTOR__POOL__H

#include "global/pool_template.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace item {

class ItemActor;

class ItemActorPool : public global::SingletonFactory<ItemActorPool> {
  friend class global::SingletonFactory<ItemActorPool>;

 public:
  bool Initialize(size_t initial_number, size_t extend_number);
  void Finalize();

  ItemActor *Allocate();
  void Deallocate(ItemActor *actor);

 private:
  ItemActorPool() {}
  ~ItemActorPool() {}

  global::PoolTemplate<ItemActor> actors_;
};

}  // namespace item

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ITEM__ITEM__ACTOR__POOL__H

