include "vocation.thrift"

namespace cpp entity
namespace py ccentity.soul
namespace as3 com.dreamfire.sanguo.data.definition.entity

enum SoulSettingID {
  MIN = 0,

  COMMON_1,                                         // 普通英灵1
  COMMON_2,                                         // 普通英灵2
  COMMON_3,                                         // 普通英灵3
  COMMON_4,                                         // 普通英灵4
  SOUL,                                             // 幻化英灵

  MAX,
}

// 英灵解锁类型
enum SoulUnlockType {
  MIN = 0,

  ITEM,                                             // 道具
  SPIRIT_LEVEL,                                     // 精灵等级
  SWORD_LEVEL,                                      // 魔剑(神武)等级
  NOBLE_LEVEL,                                      // 贵族(爵位)等级

  MAX,
}


// 英灵效果类型
enum SoulEffectType {
  MIN = 0,

  ATTR,                                             // 属性
  EFFECT,                                           // 效果
  SKILL_MAX_LEVEL,                                  // 技能等级

  MAX,
}


// 英灵属性效果加成值类型
enum SoulAttributeType {
  MIN = 0,

  MAX_HP = 0,                                       // 生命值
  MAX_MP,                                           // 魔法值
  PHYSICS_ATTACK,                                   // 物理攻击
  PHYSICS_DEFENCE,                                  // 物理防御
  MAGIC_ATTACK,                                     // 魔法攻击
  MAGIC_DEFENCE,                                    // 魔法防御
  DODGE,                                            // 闪避等级
  CRIT,                                             // 暴击等级
  IGNORE_DEFENCE,                                   // 会心等级(忽视防御)

  MAX,
}


// 能量状态
enum SoulEnergyStatus {
  MIN = 0,

  INCREMENT,                                        // 恢复增长
  EXPLOSION,                                        // 满能量
  DECREMENT,                                        // 幻化缩减

  MAX,
}


// 英灵数据
struct SoulField {
  1 : required i32 id_;                             // ID
  2 : required i32 level_;                          // 等级
  3 : required i32 step_;                           // 阶级
  4 : required i32 common_skill_level_;             // 普通技能等级
  5 : required i32 appear_skill_level_;             // 幻化技能等级
  6 : required i32 soul_skill_level_;               // 英灵技能等级
}

// 英灵设置
struct SoulSettingField {
  1 : required vocation.VocationType vocation_;     // 职业
  2 : required i32 group_;                          // 组
  3 : required list<i32> settings_;                 // 设置
}

