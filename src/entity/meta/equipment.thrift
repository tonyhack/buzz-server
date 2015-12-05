include "item.thrift"

namespace cpp entity
namespace py ccentity.equip
namespace as3 com.dreamfire.sanguo.data.definition.entity

// 装备属性效果加成值类型
enum EquipAttributeType {
  MIN = 0, 

  PHYSIQUE,                               // 体质
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

  MAX,
}


enum EquipClientFields {
  CLIENT_BEGIN = 0,

  INTENSIFY = 0,                          // 强化等级
  MAX_INTENSIFY,                          // 最高已强化等级

  RANDOM_ATTR_1,                          // 随机属性1
  RANDOM_VALUE_1,                         // 随机属性值1
  RANDOM_ATTR_2,                          // 随机属性2
  RANDOM_VALUE_2,                         // 随机属性值2
  RANDOM_ATTR_3,                          // 随机属性3
  RANDOM_VALUE_3,                         // 随机属性值3
  RANDOM_ATTR_4,                          // 随机属性4
  RANDOM_VALUE_4,                         // 随机属性值4
  RANDOM_ATTR_5,                          // 随机属性5
  RANDOM_VALUE_5,                         // 随机属性值5
  RANDOM_ATTR_6,                          // 随机属性6
  RANDOM_VALUE_6,                         // 随机属性值6

  UPGRADE_LUCKY,                          // 升级幸运值

  CLIENT_END,
}

enum EquipServerFields {
  SERVER_BEGIN = 0,

  SERVER_END,
}

struct EquipClientField {
  1 : required EquipClientFields field_;
  2 : required i32 value_;
}

struct EquipServerField {
  1 : required EquipServerFields field_;
  2 : required i32 value_;
}

// 装备外观信息
struct EquipFacadeField {
  1 : required item.EquipmentType location_;          // 装备位置
  2 : required i32 template_id_;                      // 装备模板ID
  3 : required byte intensify_;                       // 强化等级
}

