//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-20 15:29:50.
// File name: item_pool.h
//
// Description:
// Define class ItemPool.
//

#ifndef __GAME__SERVER__ITEM__ITEM__POOL__H
#define __GAME__SERVER__ITEM__ITEM__POOL__H

#include "entity/item_types.h"
#include "global/pool_template.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace item {

class Item;
class Equipment;
class Material;
class Medicine;

class ItemPool : public global::SingletonFactory<ItemPool> {
  friend class global::SingletonFactory<ItemPool>;

 public:
  bool Initialize(size_t initial_number, size_t extend_number);
  void Finalize();

  Item *Allocate(entity::ItemType::type type);
  void Deallocate(Item *item);

 private:
  ItemPool() {}
  ~ItemPool() {}

  global::PoolTemplate<Equipment> equipments_;
  global::PoolTemplate<Medicine> medicines_;
  //global::PoolTemplate<Medicine> cards_;
  global::PoolTemplate<Material> materials_;
  //global::PoolTemplate<Medicine> tasks_;

  core::uint64 sequence_;
};

}  // namespace item

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ITEM__ITEM__POOL__H

