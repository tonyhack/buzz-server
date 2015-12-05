//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-27 10:39:37.
// File name: equipment_container.h
//
// Description:
// Define class EquipmentContainer.
//

#ifndef __GAME__SERVER__ITEM__EQUIPMENT__CONTAINER__H
#define __GAME__SERVER__ITEM__EQUIPMENT__CONTAINER__H

#include <map>

#include "entity/item_types.h"
#include "game_server/server/extension/item/equipment.h"
#include "game_server/server/extension/item/item_container.h"

namespace game {

namespace server {

namespace item {

class ItemActor;

class EquipmentContainer : public ItemContainer {
  typedef std::map<int, int> SuitMap;
 public:
  using ItemContainer::CheckAddItem;

  EquipmentContainer();
  virtual ~EquipmentContainer();

  bool Initialize(ItemActor *actor, core::uint16 capacity,
      core::uint16 max_capacity, entity::VocationType::type vocation);
  void Finalize();

  virtual bool InitializeItems(const ItemFieldVector &items);
  virtual void Save() {}

  inline bool CheckLoadFinish() const {
    return this->load_finish_;
  }

  inline void SetLoadFinish(bool finish = true) {
    this->load_finish_ = finish;
  }

  virtual entity::ItemContainerType::type GetType() const {
    return entity::ItemContainerType::EQUIP;
  }

  // 增加一个道具
  virtual bool CheckAddItem(Item *item, core::uint16 index) const;
  virtual bool AddItem(Item *item, core::uint16 index, bool sync = true, bool broadcast = true);

  // 移除一个道具
  virtual bool CheckRemoveItem(Item *item) const;
  virtual bool RemoveItem(Item *item, bool sync = true, bool broadcast = true);

  // 禁用函数
  virtual bool CheckAddItem(core::uint32 template_id, core::uint32 number, bool bind) const {
    return false;
  }
  virtual bool CheckAddItems(std::vector<entity::AddItemFields>& checklists) const {
    return false;
  }
  virtual bool AddItems(core::uint32 template_id, core::uint32 number, bool bind,
      core::int32 intensify_level, const std::string *extra_info = NULL) {
    return false;
  }
  virtual bool AddItemsByLocation(core::uint16 location, core::uint32 template_id,
      core::uint32 number, bool simulate, bool bind, core::int32 intensify_level, const std::string *extra_info = NULL) {
    return false;
  }

  core::int32 OverQualityEquip(core::int32 quality);

  core::int32 OverStrengthLevelEquip(core::int32 level);

  // 装备栏不能整理
  virtual void Arrange() {}

  // 过期检测
  void ExpireCheck();

  // 开启/关闭
  void Stand();
  void Sit();

  // 更新装备战斗力
  void UpdateEquipmentFightScore(bool sync = true);

 private:
  // 装备生效
  void EquipSuitStand(Equipment *item);
  // 装备取消生效
  void EquipSuitSit(Equipment *item);
  
  // 装备栏套装信息
  SuitMap suits_;
  // 是否加载完成
  bool load_finish_;
};

}  // namespace item

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ITEM__EQUIPMENT__CONTAINER__H

