//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-27 19:08:57.
// File name: equipment.h
//
// Description:
// Define class Equipment.
//

#ifndef __GAME__SERVER__ITEM__EQUIPMENT__H
#define __GAME__SERVER__ITEM__EQUIPMENT__H

#include "entity/equipment_types.h"
#include "game_server/server/extension/item/item.h"

namespace game {

namespace server {

namespace item {

class EquipmentCell;

class Equipment : public Item {
  typedef std::vector<int32_t> AttributeFields;

  typedef void (*AttributeReviseFunctor)(core::int32 &value);

 public:
  using Item::SetAttribute;
  using Item::GetAttribute;
  using Item::Initialize;
  using Item::Finalize;

  Equipment();
  virtual ~Equipment();

  void Allocate(core::uint64 id);
  void Deallocate();

  virtual bool Initialize(core::uint32 template_id, ItemActor *actor);
  virtual void Finalize();

  // 从数据库初始化
  virtual void InitializeDatabase(const database::protocol::StorageItemField &field);

  // 导出数据库存储字段
  virtual void ExportDatabaseField(database::protocol::StorageItemField &field);

  // 导出初始化字段
  virtual void ExportInitialField(entity::ItemFields &field) const;

  // 导入道具额外信息
  virtual void ImportExtraInfo(const std::string &input);
  // 导出道具额外信息
  virtual void ExportExtraInfo(std::string &output) const;

  // 道具类型
  virtual entity::ItemType::type GetItemType() const {
    return entity::ItemType::EQUIPMENT;
  }

  virtual bool CheckExpire() const { return this->expire_; }

  bool ExpireCheck();

  // 得到装备类型
  entity::EquipmentType::type GetEquipType() const;

  // 私有可见属性
  core::int32 GetAttribute(entity::EquipClientFields::type type) const;
  bool SetAttribute(entity::EquipClientFields::type type, core::int32 value,
      bool synchronize = true);

  // 服务器属性
  core::int32 GetAttribute(entity::EquipServerFields::type type) const;
  bool SetAttribute(entity::EquipServerFields::type type, core::int32 value,
      bool synchronize = true);

  inline const AttributeFields &GetEquipClientAttributes() const {
    return this->client_attribute_;
  }

  // 同步属性
  virtual void Synchronize();
  virtual void Synchronize(entity::EquipClientFields::type type);

  // 属性修正
  void ReviseAttribute(entity::EquipClientFields::type type);
  void ReviseAttribute(entity::EquipServerFields::type type);

  // 更新广播到周围(外观)
  void UpdateBroadcast(entity::EquipmentType::type location, bool equip);

  // 装备加载/卸载
  bool OnLoadEquip(entity::EquipmentType::type location, bool synchronize = true);
  bool UnloadEquip(entity::EquipmentType::type location, bool synchronize = true);

  // 开启/关闭 主属性
  void StartMainEffect(const EquipmentCell *cell);
  void StopMainEffect(const EquipmentCell *cell);

  // 开启/关闭 副属性
  void StartSecondEffect(const EquipmentCell *cell);
  void StopSecondEffect(const EquipmentCell *cell);

  // 开启/关闭 强化效果
  void StartIntensify(const EquipmentCell *cell);
  void StopIntensify(const EquipmentCell *cell);

  // 开启/关闭 随机属性
  void StartRandEffect(const EquipmentCell *cell);
  void StopRandEffect(const EquipmentCell *cell);

 private:
  // 开启/关闭 效果
  void StartEffect(entity::EquipAttributeType::type type, core::int32 value);
  void StopEffect(entity::EquipAttributeType::type type, core::int32 value);

  AttributeFields client_attribute_;
  AttributeFields server_attribute_;

  // 首次装备时间
  time_t first_equip_time_;

  // 是否过期
  bool expire_;
};

}  // namespace item

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ITEM__EQUIPMENT__H

