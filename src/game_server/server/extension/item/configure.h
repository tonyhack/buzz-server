//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-22 10:56:31.
// File name: configure.h
//
// Description:
// Define class Configure.
//

#ifndef __GAME__SERVER__ITEM__CONFIGURE__H
#define __GAME__SERVER__ITEM__CONFIGURE__H

#include <string>

#include "core/base/types.h"
#include "entity/item_types.h"
#include "game_server/server/extension/item/item_configure.h"
#include "game_server/server/extension/item/load_equipment_configure.h"
#include "game_server/server/extension/item/load_equip_refine_configure.h"
#include "game_server/server/extension/item/load_gem_configure.h"
#include "game_server/server/extension/item/load_hunt_treasure_configure.h"
#include "game_server/server/extension/item/load_intensify_configure.h"
#include "game_server/server/extension/item/load_item_compose_configure.h"
#include "game_server/server/extension/item/load_mall_configure.h"
#include "game_server/server/extension/item/load_material_configure.h"
#include "game_server/server/extension/item/load_medicine_configure.h"
#include "game_server/server/extension/item/load_vip_gifts_configure.h"
#include "game_server/server/extension/item/load_random_attribute.h"
#include "game_server/server/extension/item/load_shop_configure.h"
#include "game_server/server/extension/item/load_suit_configure.h"
#include "game_server/server/extension/item/load_treasure_spend_configure.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace item {

class Configure : public global::SingletonFactory<Configure> {
  friend class global::SingletonFactory<Configure>;

 public:
  bool LoadConfigure(const std::string &file);

  // 获取材料配置
  const MaterialCell *GetMaterialConfigure(core::uint32 id) const;

  // 获取药品配置
  const MedicineCell *GetMedicineConfigure(core::uint32 id) const;

  // 获取装备配置
  const EquipmentCell *GetEquipmentConfigure(core::uint32 id) const;

  // 获取物品配置
  const ItemCell *GetItem(core::uint32 id) const;

  // 随机出一个属性
  const RandomAttributeCell *GetRandomAttribute(core::uint32 random_id) const;

  // 获取强化配置
  const IntensifyCell *GetIntensifyConfigure(core::int32 equip_level,
      core::int32 intensify) const;

  // 获取洗练消耗配置
  const EquipRefineSpendCell *GetRefineConfigure(core::int32 lv, core::int32 quality) const;
  
  // 获取洗练加锁配置
  const EquipRefineSpendLockCell *GetRefineLockConfigure(core::int32 lock_num) const;

  // 获取加载随机配置对象
  const LoadRandomAttribute &GetLoadRandomeAttribute() {
    return this->load_random_attribute_;
  }
  // 获取商店配置
  bool CheckShopTemplateID(core::int32 id);

  // 获取宝石配置
  const GemCell *GetGem(core::int32 id, core::int32 level) const;
  core::int32 GetRandomGemIDByQuality(GemCell::QualityType::type type) const;

  // 获取VIP礼包配置
  const LoadVipGiftsConfigure::Awards *GetAwards(entity::VipType::type type) const;

  // 获取宝石激活点配置
  const GemGatheringPointCell *GetGemGatheringPoint(core::int32 id) const;
  const GemGatheringPointCell *GetNextGemGatheringPoint(core::int32 id) const;
  const GemGatheringPointCell *GetPrevGemGatheringPoint(core::int32 id) const;
  const GemGatheringPointCell *GetLastGemGatheringPoint() const;

  // 获取商城配置
  LoadMallConfigure &GetMallConfigure() {
    return this->load_mall_;
  }

  // 获取道具合成配方配置
  const ItemComposeRecipeCell *GetItemComposeRecipe(core::int32 id) const;

  // 获取套装配置
  const LoadSuitConfigure::EffectVecs *GetSuit(core::int32 suit_id) const;

  // 获取寻宝掉落
  HuntTreasureRandCell *GetTreasureRandDrop(core::int32 tab_id);

  // 获取寻宝消耗
  const TreasureSpendCell *GetSpendCell(core::int32 id, core::int32 num) const;

  // 获取标签页对应的积分类型
  IntegralType::type GetItegralType(core::int32 tab_id);

  // 物品对象池初始化数量/扩展数量
  int item_initial_number_;
  int item_extend_number_;

  static const float kIntensifyVar1_;
  static const float kIntensifyVar2_;

 private:
  Configure() {}
  ~Configure() {}

  LoadMaterialConfigure load_material_configure_;
  LoadMedicineConfigure load_medicine_configure_;
  LoadEquipmentConfigure load_equipment_configure_;
  LoadRandomAttribute load_random_attribute_;
  LoadIntensifyConfigure load_intensify_;
  LoadEquipRefineConfigure load_refine_;
  LoadGemConfigure load_gem_;
  LoadShopConfigure load_shop_;
  LoadMallConfigure load_mall_;
  LoadVipGiftsConfigure load_vip_gifts_;
  LoadItemComposeConfigure load_item_compose_recipe_;
  LoadSuitConfigure load_suit_configure_;
  LoadTreasureSpendConfigure load_treasure_spend_;
  LoadHuntTreasureConfigure load_hunt_treasure_;
};

}  // namespace item

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ITEM__CONFIGURE__H

