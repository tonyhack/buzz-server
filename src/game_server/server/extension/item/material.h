//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-18 20:22:05.
// File name: material.h
//
// Description:
// Define class Material.
//

#ifndef __GAME__SERVER__ITEM__MATERIAL__H
#define __GAME__SERVER__ITEM__MATERIAL__H

#include "game_server/server/extension/item/item.h"

namespace game {

namespace server {

namespace item {

class Material : public Item {
 public:
  using Item::SetAttribute;
  using Item::GetAttribute;
  using Item::Initialize;
  using Item::Finalize;

  Material();
  virtual ~Material();

  void Allocate(core::uint64 id);
  void Deallocate();

  virtual bool Initialize(core::uint32 template_id, ItemActor *actor);
  virtual void Finalize();

  virtual void InitializeDatabase(const database::protocol::StorageItemField &field);

  virtual void ExportDatabaseField(database::protocol::StorageItemField &field);

  virtual void ExportInitialField(entity::ItemFields &field) const;

  // 道具类型
  virtual entity::ItemType::type GetItemType() const {
    return entity::ItemType::MATERIAL;
  }

  virtual void Synchronize();

  entity::MaterialType::type GetEquipType() const;
};

}  // namespace item

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ITEM__MATERIAL__H

