//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-20 17:38:00.
// File name: medicine.h
//
// Description:
// Define class Medicine.
//

#ifndef __GAME__SERVER__ITEM__MEDICINE__H
#define __GAME__SERVER__ITEM__MEDICINE__H

#include "game_server/server/extension/item/item.h"

namespace game {

namespace server {

namespace item {

class ItemCell;
class MedicineCell;

class Medicine : public Item {
 public:
  using Item::SetAttribute;
  using Item::GetAttribute;
  using Item::Initialize;
  using Item::Finalize;

  typedef std::vector<entity::AddItemFields> ItemVector;

  Medicine();
  virtual ~Medicine();

  void Allocate(core::uint64 id);
  void Deallocate();

  virtual bool Initialize(core::uint32 template_id, ItemActor *actor);
  virtual void Finalize();

  // 从数据库初始化
  virtual void InitializeDatabase(const database::protocol::StorageItemField &field);

  // 导出数据库存储字段
  virtual void ExportDatabaseField(database::protocol::StorageItemField &field);

  virtual void ExportInitialField(entity::ItemFields &field) const;

  // 道具类型
  virtual entity::ItemType::type GetItemType() const {
    return entity::ItemType::MEDICINE;
  }

  virtual void Synchronize();
  
  // 使用道具过程
  core::int32 UseResult(const ItemCell* cell);

 private:
  core::int32 UseAddBuff(const MedicineCell *cell);
  core::int32 UseAddItem(const MedicineCell *cell);
  core::int32 UseAddSoul(const MedicineCell *cell);
  core::int32 UseAddEXP(const MedicineCell *cell); 
  core::int32 UseAddHP(const MedicineCell *cell); 
  core::int32 UseAddMP(const MedicineCell *cell);
  core::int32 UseAddNimbus(const MedicineCell *cell);
  core::int32 UseOpenRisk(const MedicineCell *cell);
  core::int32 UseOpenTrip(const MedicineCell *cell);
  core::int32 UseVip(const MedicineCell *cell);
  core::int32 OpenFunctionality(const MedicineCell *cell);
  core::int32 UseAddEnergy(const MedicineCell *cell);
  core::int32 UseAddHpTasliman(const MedicineCell *cell);
  core::int32 UseAddMpTasliman(const MedicineCell *cell);

  static ItemVector items_;
};

}  // namespace item

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ITEM__MEDICINE__H

