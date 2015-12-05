//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-20 10:27:58.
// File name: item_actor_manager.h
//
// Description:
// Define class ItemActorManager.
//

#ifndef __GAME__SERVER__ITEM__ITEM__ACTOR__MANAGER__H
#define __GAME__SERVER__ITEM__ITEM__ACTOR__MANAGER__H

#include <ext/hash_map>

#include "core/base/types.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace item {

class ItemActor;

class ItemActorManager : public global::SingletonFactory<ItemActorManager> {
  friend class global::SingletonFactory<ItemActorManager>;

  typedef __gnu_cxx::hash_map<core::uint64, ItemActor *> ActorHashmap;

 public:
  ItemActor *Get(core::uint64 id);
  bool Add(ItemActor *actor);
  ItemActor *Remove(core::uint64 id);
  void DailyClean(core::int32 days);

 private:
  ItemActorManager() {}
  ~ItemActorManager() {}

  ActorHashmap actors_;
};

}  // namespace item

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ITEM__ITEM__ACTOR__MANAGER__H

