//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-28 15:34:50.
// File name: item_builder.h
//
// Description:
// Define class ItemBuilder.
//

#ifndef __GAME__SERVER__ITEM__ITEM__BUILDER__H
#define __GAME__SERVER__ITEM__ITEM__BUILDER__H

#include "core/base/types.h"
#include "database_server/protocol/storage_item_login_types.h"
#include "entity/item_types.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace item {

class Equipment;
class Item;
class ItemActor;
class Material;
class Medicine;

class ItemBuilder : public global::SingletonFactory<ItemBuilder> {
  friend class global::SingletonFactory<ItemBuilder>;

 public:
  Item *Create(ItemActor *actor, core::uint32 template_id);
  Item *Create(ItemActor *actor, entity::ItemType::type type,
      core::uint32 template_id);
  Item *Create(ItemActor *actor, const database::protocol::StorageItemField &field);

  void Destory(Item *item);

 private:
  Material *CreateMaterial(ItemActor *actor, core::uint32 template_id);
  Medicine *CreateMedicine(ItemActor *actor, core::uint32 template_id);
  Equipment *CreateEquipment(ItemActor *actor, core::uint32 template_id);

  Material *CreateMaterial(ItemActor *actor,
      const database::protocol::StorageItemField &field);
  Medicine *CreateMedicine(ItemActor *actor,
      const database::protocol::StorageItemField &field);
  Equipment *CreateEquipment(ItemActor *actor,
      const database::protocol::StorageItemField &field);

  ItemBuilder() {}
  ~ItemBuilder() {}
};

}  // namespace item

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ITEM__ITEM__BUILDER__H

