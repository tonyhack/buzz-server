namespace cpp entity
namespace py ccentity.skill
namespace as3 com.dreamfire.sanguo.data.definition.entity


////////////////////////////////////////
// 技能命中类型
enum SkillHitType {
  COMMON = 0,                               // 普通
  CRIT,                                     // 暴击
  DODGE,                                    // 闪避
  IGNORE_DEF,                               // 忽视防御

  MAX,
}


////////////////////////////////////////
// 伤害类型
enum SkillHurtType {
  MIN = 0,

  PHYSICS_ATTACK,                           // 物理攻击
  MAGIC_ATTACK,                             // 魔法攻击
  REDUCE_HP,                                // 直接减血
  REDUCE_MP,                                // 直接减魔
  PHYSICS_CURE,                             // 物理治疗
  MAGIC_CURE,                               // 魔法治疗
  ADD_HP,                                   // 直接加血
  ADD_MP,                                   // 直接加魔
  CLEAR_BUFF,                               // 清除BUFF
  CLEAR_BUFF_GROUP,                         // 清除BUFF组

  MAX,
}


////////////////////////////////////////
// 加BUFF对象
enum BuffTargetType {
  SELF,                                     // 对自己
  OTHER,                                    // 对其他
}


////////////////////////////////////////
// 技能状态
enum SkillFormType {
  MIN = 0,

  COMMON,                                   // 普通状态
  TEMP,                                     // 临时状态

  MAX,
}


////////////////////////////////////////
// 技能属性
enum SkillVarFields {
  SKILL_BEGIN = 0,

  COOLING_ADD_VALUE = 0,                    // 冷却时间加成值
  DISTANCE_ADD_VALUE,                       // 施法距离加成值
  SING_TIME_ADD_VALUE,                      // 吟唱时间加成值
  AOE_MAX_NUM_ADD_VALUE,                    // AOE个数加成值
  AOE_RADIUS_ADD_VALUE,                     // AOE半径加成值
  HP_SPEND_ADD_VALUE,                       // HP消耗加成值
  HP_SPEND_ADD_PERCENT,                     // HP消耗加成比(万分之)
  MP_SPEND_ADD_VALUE,                       // MP消耗加成值
  MP_SPEND_ADD_PERCENT,                     // MP消耗加成比(万分之)
  HURT_ADD_VALUE,                           // 伤害加成值
  HURT_ADD_PERCENT,                         // 伤害加成比(万分之)

  SKILL_END,
}


////////////////////////////////////////
// 技能属性字段
struct SkillVarField {
  1 : required SkillVarFields type_;        // 类型
  2 : required i32 value_;                  // 值
}

