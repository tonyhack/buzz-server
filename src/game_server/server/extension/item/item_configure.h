//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-29 12:18:57.
// File name: item_configure.h
//
// Description:
// Define class ItemConfigure.
//

#ifndef __GAME__SERVER__ITEM__ITEM__CONFIGURE__H
#define __GAME__SERVER__ITEM__ITEM__CONFIGURE__H

#include <map>
#include <string>
#include <vector>

#include "core/base/types.h"
#include "entity/item_types.h"
#include "entity/resource_types.h"

namespace game {

namespace server {

namespace item {

class ItemCell {
 public:
  ItemCell() {}
  virtual ~ItemCell() {}

  std::string name_;                  // 名称
  core::uint32 type_;                 // 类型
  core::uint32 level_;                // 使用最小等级
  core::uint32 expire_get_;           // 过期时间(得到后计时)
  time_t expire_absolute_time_;       // 绝对过期时间
  core::uint32 expire_use_;           // 过期时间(使用后计时) 
  core::uint32 buy_coin_;             // 价格(铜币)
  core::uint32 buy_gold_;             // 价格(元宝)
  core::uint32 buy_lockedgold_;       // 价格(礼金)
  core::uint32 buy_honour_;           // 价格(战功)
  core::uint32 sell_coin_;            // 卖出价格(铜币)
  core::uint32 sell_drop_;            // 卖出掉落
  core::uint32 quality_;              // 品质
  core::uint32 operate_right_;        // 操作权限(销毁、交易、收购、出售、仓库、可否绑定、可否批量使用)
  core::uint32 stack_count_;          // 叠加上限
  core::uint32 use_state_;            // 使用状态(0:普通状态 1:死亡状态 2:无需求)
  core::uint32 cooling_id_;           // 冷却时间
};

class MedicineParams {
 public:
  MedicineParams() {}
  ~MedicineParams() {}

  core::uint32 param1_;               // 参数1
  core::uint32 param2_;               // 参数2
  core::uint32 param3_;               // 参数3
};

class MedicineCell : public ItemCell {
 public:
  typedef std::vector<MedicineParams> FunctionParamsVector;

  MedicineCell() { this->type_ = entity::ItemType::MEDICINE; }
  virtual ~MedicineCell() {}

  core::int32 template_id_;                       // 道具模板ID
  core::uint32 sub_type_;                         // 子类型
  core::uint32 fighting_status_forbid_;           // 战斗状态禁止(0: 不禁止使用, 1: 禁止使用)
  core::uint32 pvp_status_forbid_;                // PVP状态禁止(0: 不禁止使用, 1: 禁止使用
  core::uint32 func_type_;                        // 功能类型
  FunctionParamsVector func_params_;              // 功能参数
  std::string func_python_;                       // python功能参数
};

class MaterialCell : public ItemCell {
 public:
  MaterialCell() { this->type_ = entity::ItemType::MATERIAL; }
  virtual ~MaterialCell() {}

  core::uint32 sub_type_;                         // 子类型
};

class EquipmentCell : public ItemCell {
 public:
  typedef std::pair<core::uint8, core::uint32> AttrPair;
  typedef std::vector<AttrPair> AttrVector;

  EquipmentCell() { this->type_ = entity::ItemType::EQUIPMENT; }
  virtual ~EquipmentCell() {}

  core::uint32 sub_type_;                         // 子类型
  core::int32 vocation_;                          // 职业
  core::uint8 gender_;                            // 性别
  bool equip_bind_;                               // 装备后是否绑定
  AttrVector attrs_;                              // 装备主属性
  AttrVector second_attrs_;                       // 装备副属性
  AttrVector intensify_five_;                     // 5星属性
  AttrVector intensify_ten_;                      // 10星属性
  core::uint32 max_intensify_;                    // 最大强化数(星数)
  core::uint32 random_attr_id_;                   // 随机属性ID
  core::uint32 random_attr_number_;               // 随机属性个数
  core::uint32 upgrade_id_;                       // 升级后的装备ID
  core::uint32 upgrade_spend_item_;               // 升级需要消耗的道具ID
  core::uint32 upgrade_spend_item_num_;           // 升级需要消耗的道具数量
  core::uint32 upgrade_spend_gold_;               // 升级需要消耗的金币数量
  core::uint32 upgrade_lucky_;                    // 幸运点数
  core::uint32 upgrade_max_lucky_;                // 升级最大幸运值
  core::uint32 upgrade_lucky_coe_;                // 升级幸运系数
  core::uint32 upgrade_odds_;                     // 升级基础成功率
  core::int32 split_id_;                          // 装备拆分ID
  core::int32 suit_id_;                           // 套装ID
};

class RandomAttributeCell {
 public:
  RandomAttributeCell() {}
  ~RandomAttributeCell() {}

  core::int16 odds_;                  // 机率
  core::uint8 type_;                  // 属性类型
  core::uint32 value_;                // 加成值
};

class IntensifyCell {
 public:
  core::int32 min_odds_;              // 最小机率
  core::int32 max_odds_;              // 最大机率
  core::int32 spend_item_;            // 消耗道具
  core::int32 spend_item_num_;        // 消耗道具数量
  core::int32 spend_gold_;            // 消耗金币数量
};

class EquipRefineSpendCell {
 public:
  core::int32 equip_lv_;              // 装备等级
  core::int32 equip_quality_;         // 装备品质
  core::int32 spend_gold_;            // 花费的金币数目
  core::int32 spend_item_;            // 花费的物品
  core::int32 spend_item_num_;        // 花费的物品数目
};

class EquipRefineSpendLockCell {
 public:
  core::int32 lock_num_;              // 锁住随机属性的数目
  core::int32 spend_diamond_;         // 消耗的钻石
};

class GemCell {
 public:
  struct QualityType {
    enum type {
      MIN = 2,

      GREEN = MIN,
      BLUE,
      PURPLE,
      ORANGE,

      MAX = ORANGE
    };
  };

  struct AttrType {
    enum type {
      MIN = 1,

      PHYSIQUE = MIN,                         // 体质
      STRENGTH,                               // 力量
      INTELLIGENCE,                           // 智力
      AGILE,                                  // 敏捷
      SPIRIT,                                 // 精神
      MAX_HP,                                 // 最大生命值
      MAX_MP,                                 // 最大魔法值
      PHYSICS_ATTACK,                         // 物理攻击
      PHYSICS_DEFENCE,                        // 物理防御
      MAGIC_ATTACK,                           // 魔法攻击
      MAGIC_DEFENCE,                          // 魔法防御
      DODGE,                                  // 闪避等级
      CRIT,                                   // 暴击等级
      IGNORE_DEFENCE,                         // 会心等级(忽视防御)
      MOVE_SPEED,                             // 移动速度

      MAX = MOVE_SPEED
    };
  };

  core::int32 id_;                             // 宝石ID
  core::int32 level_;                          // 宝石等级
  QualityType::type quality_;                  // 宝石品质
  core::int32 exp_;                            // 升级需要经验
  core::int32 promote_spend_shard_count_;      // 单次升级需要宝石碎片数量
  core::int32 promote_exp_reward_;             // 单次升级获取经验
  AttrType::type attr_id_;                     // 加成属性ID
  core::int32 attr_value_;                     // 加成属性值
  core::int32 decompose_get_shard_count_;      // 拆解得到的宝石碎片数
  core::int32 shard_per_exp_;                  // 每N点经验获得一块宝石碎片
};

class GemGatheringResultCell {
 public:
  struct BonusType {
    enum type {
      MIN = 1,

      GOLD = MIN,
      GEM_SHARD,
      GREEN_QUALITY_GEM,
      BLUE_QUALITY_GEM,
      PURPLE_QUALITY_GEM,
      ORANGE_QUALITY_GEM,

      MAX = ORANGE_QUALITY_GEM
    };
  };

  core::int32 result_id_;                      // 激活结果ID
  core::uint32 odds_;                          // 激活结果几率(万分之)
  BonusType::type bonus_id_;                   // 奖励ID
  core::int32 bonus_count_;                    // 奖励数量
};

class GemGatheringPointCell {
 public:
  core::int32 id_;                             // 激活点ID
  core::int32 spend_gold_;                     // 激活消耗金币
  bool skip_;                                  // 是否可直接到达
  core::int32 spend_freedom_dollars_;          // 直接到达消耗钻石
  std::vector<GemGatheringResultCell> results; // 激活结果集合
  core::uint32 enter_next_point_odds_;         // 进入下一级激活点几率(万分之)
};

class ItemComposeRecipeCell {
 public:
  struct BindType {
    enum type {
      MIN = 0,

      NOT_BIND = 0,                            // 不绑定
      BIND = 1,                                // 绑定
      DEPEND_ON_MATERIAL = 2,                  // 根据材料绑定情况决定是否绑定

      MAX,
    };
  };

  typedef std::map<core::uint32, core::int32> ItemMap;
  typedef std::vector<core::uint32> ItemVector;

  core::int32 id_;                             // 配方ID
  core::uint32 result_item_id_;                // 成品ID
  ItemMap consume_items_;                      // 消耗道具
  ItemVector consume_equipments_;              // 消耗装备
  core::int32 resource_id_;                    // 资源ID(0表示不耗费任何资源)
  core::int32 resource_number_;                // 资源数量
  BindType::type bind_;                        // 绑定规则
};

}  // namespace item

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ITEM__ITEM__CONFIGURE__H

