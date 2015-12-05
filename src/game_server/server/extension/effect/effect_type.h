//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-02 19:19:11.
// File name: effect_type.h
//
// Description:
// Define effect type.
//

#ifndef __GAME__SERVER__EFFECT__EFFECT__TYPE__H
#define __GAME__SERVER__EFFECT__EFFECT__TYPE__H

#include <stddef.h>

namespace game {

namespace server {

namespace effect {

struct EffectType {
  enum type {
    EFFECT_MIN = 0,

    EFFECT_ATTRIBUTE = 0,                 // 修改属性
    EFFECT_STATUS,                        // 状态
    EFFECT_CLEARING,                      // 驱散BUFF
    EFFECT_SKILL,                         // 修改技能属性

    EFFECT_MAX,
  };
};

// 效果操作数类型
struct EffectOperandType {
  enum type {
    OPERAND_NUMBER = 0,                   // 数值
    OPERAND_PERCENT,                      // 百分比(单位定义为万分之几)

    OPERAND_MAX,
  };
};

// 应该对应 目录 “entity/meta/” 下的 entity.thrift/role.thrift/actor.thrift 属性
struct EffectAttributeType {
  enum type {
    ENTITY_CLIENT_BEGIN = 0,
    ENTITY_CLIENT_END = 0,

    ENTITY_SERVER_BEGIN = 1000,
    ENTITY_SERVER_END = 1000,


    ROLE_AOI_BEGIN = 2000,
    LEVEL = 2000,                                 // 当前等级[消耗类属性]
    VOCATION = 2001,                              // 当前职业[被动类属性]
    CURRENT_HP = 2002,                            // 当前血量值[消耗类属性]
    MAX_HP = 2003,                                // 最大血量值[被动类属性]
    CURRENT_MP = 2004,                            // 当前魔法值[消耗类属性]
    MAX_MP = 2005,                                // 最大魔法值[被动类属性]
    MOVE_SPEED = 2006,                            // 移动速度[被动类属性]
    STATUS = 2007,                                // 状态字段[被动类属性]
    COMMON_STATUS = 2008,                         // 常见状态[被动类属性]
    CAMP = 2009,                                  // 阵营[被动类属性]
    ROLE_AOI_END = 2009,

    ROLE_CLIENT_BEGIN = 3000,
    PHYSIQUE = 3000,                              // 体质[被动类属性]
    STRENGTH = 3001,                              // 力量[被动类属性]
    INTELLIGENCE = 3002,                          // 智力[被动类属性]
    AGILE = 3003,                                 // 敏捷[被动类属性]
    SPIRIT = 3004,                                // 精神[被动类属性]
    PHYSICS_ATTACK = 3005,                        // 物理攻击[被动类属性]
    PHYSICS_DEFENCE = 3006,                       // 物理防御[被动类属性]
    MAGIC_ATTACK = 3007,                          // 魔法攻击[被动类属性]
    MAGIC_DEFENCE = 3008,                         // 魔法防御[被动类属性]
    DODGE = 3009,                                 // 闪避等级[被动类属性]
    CRIT = 3010,                                  // 暴击等级[被动类属性]
    IGNORE_DEFENCE = 3011,                        // 会心等级[被动类属性]
    CURRENT_ENERGY = 3012,                        // 当前能量值[消耗类属性]
    MAX_ENERGY = 3013,                            // 最大能量值[被动类属性]
    ENERGY_INCREMENT = 3014,                      // 单位时间能量恢复值[被动类属性]
    ROLE_CLIENT_END = 3014,

    ROLE_SERVER_BEGIN = 4000,
    MAX_HP_ADD_VALUE = 4000,                      // 最大血量加成值
    MAX_HP_ADD_SECOND_VALUE = 4001,               // 最大血量加成值
    MAX_HP_ADD_PERCENT = 4002,                    // 最大血量加成比(万分之)
    MAX_MP_ADD_VALUE = 4003,                      // 最大魔法加成值
    MAX_MP_ADD_SECOND_VALUE = 4004,               // 最大魔法加成值
    MAX_MP_ADD_PERCENT = 4005,                    // 最大魔法加成比(万分之)
    MOVE_SPEED_ADD_VALUE = 4006,                  // 移动速度加成值
    MOVE_SPEED_ADD_SECOND_VALUE = 4007,           // 移动速度加成值
    MOVE_SPEED_ADD_PERCENT = 4008,                // 移动速度加成比(万分之)
    PHYSIQUE_ADD_VALUE = 4009,                    // 体质加成值
    PHYSIQUE_ADD_SECOND_VALUE = 4010,             // 体质加成值
    PHYSIQUE_ADD_PERCENT = 4011,                  // 体质加成比(万分之)
    STRENGTH_ADD_VALUE = 4012,                    // 力量加成值
    STRENGTH_ADD_SECOND_VALUE = 4013,             // 力量加成值
    STRENGTH_ADD_PERCENT = 4014,                  // 力量加成比(万分之)
    INTELLIGENCE_ADD_VALUE = 4015,                // 智力加成值
    INTELLIGENCE_ADD_SECOND_VALUE = 4016,         // 智力加成值
    INTELLIGENCE_ADD_PERCENT = 4017,              // 智力加成比(万分之)
    AGILE_ADD_VALUE = 4018,                       // 敏捷加成值
    AGILE_ADD_SECOND_VALUE = 4019,                // 敏捷加成值
    AGILE_ADD_PERCENT = 4020,                     // 敏捷加成比(万分之)
    SPIRIT_ADD_VALUE = 4021,                      // 精神加成值
    SPIRIT_ADD_SECOND_VALUE = 4022,               // 精神加成值
    SPIRIT_ADD_PERCENT = 4023,                    // 精神加成比(万分之)
    PHYSICS_ATTACK_ADD_VALUE = 4024,              // 物理攻击加成值
    PHYSICS_ATTACK_ADD_SECOND_VALUE = 4025,       // 物理攻击加成值
    PHYSICS_ATTACK_ADD_PERCENT = 4026,            // 物理攻击加成比(万分之)
    PHYSICS_DEFENCE_ADD_VALUE = 4027,             // 物理防御加成值
    PHYSICS_DEFENCE_ADD_SECOND_VALUE = 4028,      // 物理防御加成值
    PHYSICS_DEFENCE_ADD_PERCENT = 4029,           // 物理防御加成比(万分之)
    MAGIC_ATTACK_ADD_VALUE = 4030,                // 魔法攻击加成值
    MAGIC_ATTACK_ADD_SECOND_VALUE = 4031,         // 魔法攻击加成值
    MAGIC_ATTACK_ADD_PERCENT = 4032,              // 魔法攻击加成比(万分之)
    MAGIC_DEFENCE_ADD_VALUE = 4033,               // 魔法防御加成值
    MAGIC_DEFENCE_ADD_SECOND_VALUE = 4034,        // 魔法防御加成值
    MAGIC_DEFENCE_ADD_PERCENT = 4035,             // 魔法防御加成比(万分之)
    DODGE_ADD_VALUE = 4036,                       // 闪避等级加成值
    DODGE_ADD_SECOND_VALUE = 4037,                // 闪避等级加成值
    DODGE_ADD_PERCENT = 4038,                     // 闪避等级加成比(万分之)
    DODGE_PERCENT_ADD_VALUE = 4039,               // 闪避率加成值(万分之)
    CRIT_ADD_VALUE = 4040,                        // 暴击等级加成值
    CRIT_ADD_SECOND_VALUE = 4041,                 // 暴击等级加成值
    CRIT_ADD_PERCENT = 4042,                      // 暴击等级加成比(万分之)
    CRIT_PERCENT_ADD_VALUE = 4043,                // 暴击率加成值(万分之)
    CRIT_EFFECT_PERCENT_ADD_VALUE = 4044,         // 暴击效果加成值(万分之)
    IGNORE_DEFENCE_ADD_VALUE = 4045,              // 会心等级加成值
    IGNORE_DEFENCE_ADD_SECOND_VALUE = 4046,       // 会心等级加成值
    IGNORE_DEFENCE_ADD_PERCENT = 4047,            // 会心等级加成比(万分之)
    IGNORE_DEFENCE_PERCENT_ADD_VALUE = 4048,      // 会心率加成值(万分之)
    PHYSICS_HURT_REDUCE_ADD_PERCENT = 4049,       // 物伤累计减免率(万分之)
    MAGIC_HURT_REDUCE_ADD_PERCENT = 4050,         // 法伤累计减免率(万分之)
    HURT_REDUCE_ADD_PERCENT = 4051,               // 伤害累计减免率(万分之)
    HURT_RANDOM_PERCENT_ADD = 4052,               // 伤害浮动比加成值(万分之)
    CURE_PERCENT_ADD = 4053,                      // 治疗加成比(万分之)
    MAX_ENERGY_ADD_VALUE = 4054,                  // 最大能量加成值
    MAX_ENERGY_ADD_SECOND_VALUE = 4055,           // 最大能量二级加成值
    MAX_ENERGY_ADD_PERCENT = 4056,                // 最大能量加成比(万分之)
    ENERGY_INCREMENT_ADD_VALUE = 4057,            // 能量恢复加成值
    ENERGY_INCREMENT_ADD_SECOND_VALUE = 4058,     // 能量恢复二级加成值
    ENERGY_INCREMENT_ADD_PERCENT = 4059,          // 能量恢复加成比(万分之)
    ROLE_SERVER_END = 4059,


    ACTOR_AOI_BEGIN = 5000,
    GENDER = 5000,                                // 性别[被动类属性]
    APPEARANCE = 5001,                            // 外貌[被动类属性]
    BROWN_NAME = 5002,                            // 褐名[被动类属性]
    CURRENT_SWORD = 5003,                         // 魔剑当前阶级[被动类属性]
    CURRENT_SPIRIT_FACADE = 5004,                 // 精灵当前外观[被动类属性]
    ACTOR_AOI_END = 5004,

    ACTOR_CLIENT_BEGIN = 6000,
    CURRENT_EXP = 6000,                           // 当前经验[消耗类属性]
    NIMBUS = 6001,                                // 灵气[消耗类属性]
    GOLD = 6002,                                  // 金币[消耗类属性]
    FREEDOM_DOLLARS = 6003,                       // 非绑币[消耗类属性]
    RESTRICT_DOLLARS = 6004,                      // 绑定币[消耗类属性]
    HONOUR = 6005,                                // 荣誉[消耗类属性]
    WAR_SOUL = 6006,                              // 战魂(神威)[消耗类属性]
    GEM_SHARD = 6007,                             // 宝石碎片[消耗类属性]
    PACKET_CAPACITY = 6008,                       // 包裹容量[被动类属性]
    STORAGE_CAPACITY = 6009,                      // 仓库容量[被动类属性]
    PK_MODE = 6010,                               // PK模式[被动类属性]
    FCM_TYPE = 6011,                              // 防沉迷类型[被动类属性]
    FCM_STATUS = 6012,                            // 防沉迷状态[被动类属性]
    JUSTICE = 6013,                               // 正义点[消耗类属性]
    ACTOR_CLIENT_END = 6013,

    ACTOR_SERVER_BEGIN = 7000,
    KILL_MOB_AND_ZAZEN_EXP_ADD_PERCENT = 7000,    // 打怪和冥想经验加成比(万分之)
    KILL_MOB_AND_ZAZEN_NIMBUS_ADD_PERCENT = 7001, // 打怪和冥想灵气加成比(万分之)
    ACTOR_SERVER_END = 7001,
  };
};

// 状态效果类型
struct EffectStatusType {
  enum type {
    MIN = 0,

    SOUL,                                         // 幻化
    FORBID_MOVE,                                  // 禁止移动
    FORBID_USE_SKILL,                             // 禁用技能
    FORBID_USE_ITEM,                              // 禁用物品
    FORBID_BEATEN_MAP,                            // 禁止地图PK

    MAX,
  };
};

bool IsConsumingAttribute(EffectAttributeType::type type);
bool IsPassivityAttribute(EffectAttributeType::type type);

// 效果属性类型计算码(使用取余来得到具体某个实体类型的属性索引)
static const size_t kEffectAttributeTypeMask = 1000;

}  // namespace effect

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__EFFECT__EFFECT__TYPE__H

