namespace cpp entity
namespace py ccentity.buff
namespace as3 com.dreamfire.sanguo.data.definition.entity


////////////////////////////////////////////
// BUFF类型
enum BuffType {
  MIN = 0,

  COMMON,                         // 普通类
  DOT_BUFF,                       // DOT触发BUFF类
  DOT_HURT,                       // DOT触发HURT类
  CONDITION,                      // 条件类

  MAX,
}


////////////////////////////////////////////
// 广播类型
enum BuffBroadcastType {
  MIN = 0,

  BROAD,                          // 广播
  SINGLE,                         // 只发送给自己
  NONE,                           // 不广播

  MAX,
}


////////////////////////////////////////////
// BUFF结束条件
enum BuffEndConditionType {
  MIN = 0,

  DEAD = 0x1,                     // 死亡
  HURT = 0x2,                     // 受到伤害
  CURRENT_ENERGY_DEPLETED = 0x4,  // 当前能量值耗尽

  MAX,
}


////////////////////////////////////////////
// BUFF条件
enum BuffConditionType {
  MIN = 0,

  HP_PERCENT_BELOW,               // 生命值低于p1%
  HP_PERCENT_ABOVE,               // 生命值高于p1%
  HP_PERCENT_EQUAL,               // 生命值等于p1%
  MP_PERCENT_BELOW,               // 魔法值低于p1%
  MP_PERCENT_ABOVE,               // 魔法值高于p1%
  MP_PERCENT_EQUAL,               // 魔法值等于p1%
  HP_VALUE_BELOW,                 // 生命值低于p1
  HP_VALUE_ABOVE,                 // 生命值高于p1
  HP_VALUE_EQUAL,                 // 生命值等于p1
  MP_VALUE_BELOW,                 // 魔法值低于p1
  MP_VALUE_ABOVE,                 // 魔法值高于p1
  MP_VALUE_EQUAL,                 // 魔法值等于p1
  ATTACK_HIT,                     // 攻击命中
  ATTACK_DODGE,                   // 攻击闪避
  ATTACK_CRIT,                    // 攻击暴击
  ATTACK_IGNORE_DEFENCE,          // 攻击会心
  ATTACKED_HIT,                   // 被攻击命中
  ATTACKED_DODGE,                 // 被攻击闪避
  ATTACKED_CRIT,                  // 被攻击暴击
  ATTACKED_IGNORE_DEFENCE,        // 被攻击会心
  SOUL_STATUS,                    // 幻化状态
  NOT_SOUL_STATUS,                // 非幻化状态
  FORBID_USE_SKILL,               // 禁止使用技能
  FORBID_MOVE,                    // 禁止移动
  FORBID_USE_ITEM,                // 禁止使用道具
  BUFF,                           // BUFF状态下
  BUFF_GROUP,                     // BUFF组状态下
  SKILL_USE,                      // 使用某个技能
  SKILL_GROUP_USE,                // 使用某个技能组

  MAX,
}

