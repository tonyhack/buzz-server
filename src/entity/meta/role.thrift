namespace cpp entity
namespace py ccentity.role
namespace as3 com.dreamfire.sanguo.data.definition.entity

enum RoleAoiFields {
  AOI_BEGIN = 0,

  LEVEL = 0,                              // 等级

  VOCATION,                               // 当前职业

  CURRENT_HP,                             // 当前血量值
  MAX_HP,                                 // 最大血量值

  CURRENT_MP,                             // 当前魔法值
  MAX_MP,                                 // 最大魔法值

  MOVE_SPEED,                             // 移动速度(单位为每移动一格需要多少毫秒)

  STATUS,                                 // 状态字段(第0字节:基本状态[见RoleBasicStatus])

  COMMON_STATUS,                          // 常见状态

  CAMP,                                   // 阵营

  DIRECTION,                              // 方向

  AOI_END,
}

enum RoleClientFields {
  CLIENT_BEGIN = 0,

  PHYSIQUE = 0,                           // 体质
  STRENGTH,                               // 力量
  INTELLIGENCE,                           // 智力
  AGILE,                                  // 敏捷
  SPIRIT,                                 // 精神

  PHYSICS_ATTACK,                         // 物理攻击
  PHYSICS_DEFENCE,                        // 物理防御

  MAGIC_ATTACK,                           // 魔法攻击
  MAGIC_DEFENCE,                          // 魔法防御

  DODGE,                                  // 闪避等级
  CRIT,                                   // 暴击等级
  IGNORE_DEFENCE,                         // 会心等级(忽视防御)

  CURRENT_ENERGY,                         // 当前能量值
  MAX_ENERGY,                             // 最大能量值
  ENERGY_INCREMENT,                       // 单位时间能量恢复值

  CLIENT_END,
}

enum RoleServerFields {
  SERVER_BEGIN = 0,

  MAX_HP_ADD_VALUE = 0,                   // 最大血量加成值
  MAX_HP_ADD_SECOND_VALUE,                // 最大血量二级加成值
  MAX_HP_ADD_PERCENT,                     // 最大血量加成比(万分之)

  MAX_MP_ADD_VALUE,                       // 最大魔法加成值
  MAX_MP_ADD_SECOND_VALUE,                // 最大魔法二级加成值
  MAX_MP_ADD_PERCENT,                     // 最大魔法加成比(万分之)

  MOVE_SPEED_ADD_VALUE,                   // 移动速度加成值
  MOVE_SPEED_ADD_SECOND_VALUE,            // 移动速度二级加成值
  MOVE_SPEED_ADD_PERCENT,                 // 移动速度加成比(万分之)

  PHYSIQUE_ADD_VALUE,                     // 体质加成值
  PHYSIQUE_ADD_SECOND_VALUE,              // 体质二级加成值
  PHYSIQUE_ADD_PERCENT,                   // 体质加成比(万分之)

  STRENGTH_ADD_VALUE,                     // 力量加成值
  STRENGTH_ADD_SECOND_VALUE,              // 力量二级加成值
  STRENGTH_ADD_PERCENT,                   // 力量加成比(万分之)

  INTELLIGENCE_ADD_VALUE,                 // 智力加成值
  INTELLIGENCE_ADD_SECOND_VALUE,          // 智力二级加成值
  INTELLIGENCE_ADD_PERCENT,               // 智力加成比(万分之)

  AGILE_ADD_VALUE,                        // 敏捷加成值
  AGILE_ADD_SECOND_VALUE,                 // 敏捷二级加成值
  AGILE_ADD_PERCENT,                      // 敏捷加成比(万分之)

  SPIRIT_ADD_VALUE,                       // 精神加成值
  SPIRIT_ADD_SECOND_VALUE,                // 精神二级加成值
  SPIRIT_ADD_PERCENT,                     // 精神加成比(万分之)

  PHYSICS_ATTACK_ADD_VALUE,               // 物理攻击加成值
  PHYSICS_ATTACK_ADD_SECOND_VALUE,        // 物理攻击二级加成值
  PHYSICS_ATTACK_ADD_PERCENT,             // 物理攻击加成比(万分之)

  PHYSICS_DEFENCE_ADD_VALUE,              // 物理防御加成值
  PHYSICS_DEFENCE_ADD_SECOND_VALUE,       // 物理防御二级加成值
  PHYSICS_DEFENCE_ADD_PERCENT,            // 物理防御加成比(万分之)

  MAGIC_ATTACK_ADD_VALUE,                 // 魔法攻击加成值
  MAGIC_ATTACK_ADD_SECOND_VALUE,          // 魔法攻击二级加成值
  MAGIC_ATTACK_ADD_PERCENT,               // 魔法攻击加成比(万分之)

  MAGIC_DEFENCE_ADD_VALUE,                // 魔法防御加成值
  MAGIC_DEFENCE_ADD_SECOND_VALUE,         // 魔法防御二级加成值
  MAGIC_DEFENCE_ADD_PERCENT,              // 魔法防御加成比(万分之)

  DODGE_ADD_VALUE,                        // 闪避等级加成值
  DODGE_ADD_SECOND_VALUE,                 // 闪避等级二级加成值
  DODGE_ADD_PERCENT,                      // 闪避等级加成比(万分之)
  DODGE_PERCENT_ADD_VALUE,                // 闪避率加成值(万分之)

  CRIT_ADD_VALUE,                         // 暴击等级加成值
  CRIT_ADD_SECOND_VALUE,                  // 暴击等级二级加成值
  CRIT_ADD_PERCENT,                       // 暴击等级加成比(万分之)
  CRIT_PERCENT_ADD_VALUE,                 // 暴击率加成值(万分之)
  CRIT_EFFECT_PERCENT_ADD_VALUE,          // 暴击效果加成值(万分之)

  IGNORE_DEFENCE_ADD_VALUE,               // 会心等级加成值
  IGNORE_DEFENCE_ADD_SECOND_VALUE,        // 会心等级二级加成值
  IGNORE_DEFENCE_ADD_PERCENT,             // 会心等级加成比(万分之)
  IGNORE_DEFENCE_PERCENT_ADD_VALUE,       // 会心率加成值(万分之)

  PHYSICS_HURT_REDUCE_ADD_PERCENT,        // 物伤累计减免率(万分之)
  MAGIC_HURT_REDUCE_ADD_PERCENT,          // 法伤累计减免率(万分之)
  HURT_REDUCE_ADD_PERCENT,                // 伤害累计减免率(万分之)

  HURT_RANDOM_PERCENT_ADD,                // 伤害浮动比加成值(万分之)
  CURE_PERCENT_ADD,                       // 治疗加成比(万分之)

  MAX_ENERGY_ADD_VALUE,                   // 最大能量加成值
  MAX_ENERGY_ADD_SECOND_VALUE,            // 最大能量二级加成值
  MAX_ENERGY_ADD_PERCENT,                 // 最大能量加成比(万分之)

  ENERGY_INCREMENT_ADD_VALUE,             // 能量恢复加成值
  ENERGY_INCREMENT_ADD_SECOND_VALUE,      // 能量恢复二级加成值
  ENERGY_INCREMENT_ADD_PERCENT,           // 能量恢复加成比(万分之)

  SERVER_END,
}

struct RoleAoiField {
  1 : required RoleAoiFields field_;
  2 : required i32 value_;
}

struct RoleClientField {
  1 : required RoleClientFields field_;
  2 : required i32 value_;
} 

enum RoleBasicStatus {
  STATUS_MIN = 0,

  STATUS_COMMON = 0,                      // 普通状态
  STATUS_DIE,                             // 死亡状态
  STATUS_ZAZEN,                           // 打坐状态

  STATUS_MAX,
}

enum RoleCommonStatus {
  SOUL = 0x1,                             // 幻化状态
  FORBID_MOVE = 0x2,                      // 移动禁止
  FORBID_USE_SKILL = 0x4,                 // 技能禁止
  FORBID_USE_ITEM = 0x8,                  // 物品禁止
  FORBID_BEATEN_MAP = 0x10,               // 被打禁止(某些地图中适用)
}

// 死亡后的状态
enum RoleAfterDieStatus {
  NORMAL = 0,                             // 正常状态
  PROTECT = 1,                            // 保护状态
}

