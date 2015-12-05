//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-20 14:07:32.
// File name: item_manager.h
//
// Description:
// Define class ItemManager.
//

#ifndef __GAME__SERVER__ITEM__ITEM__MANAGER__H
#define __GAME__SERVER__ITEM__ITEM__MANAGER__H

#include <ext/hash_map>

#include "core/base/types.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace item {

class Item;

class ItemManager : public global::SingletonFactory<ItemManager> {
  friend class global::SingletonFactory<ItemManager>;

  typedef __gnu_cxx::hash_map<core::uint64, Item *> ItemHashmap;

 public:
  Item *Get(core::uint64 id);
  bool Add(Item *item);
  Item *Remove(core::uint64 id);

 private:
  ItemManager() {}
  ~ItemManager() {}

  ItemHashmap items_;
};

}  // namespace item

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ITEM__ITEM__MANAGER__H

