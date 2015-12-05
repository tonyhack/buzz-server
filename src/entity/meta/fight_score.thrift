namespace cpp entity
namespace py ccentity.fight_score
namespace as3 com.dreamfire.sanguo.data.definition.entity

////////////////////////////////////////////////
// 战斗力类型定义
enum FightScoreType {
  MIN = 0,

  AXE,                                // 战士战斗力
  SHOOT,                              // 弓手战斗力
  MAGIC,                              // 法师战斗力

  MAX,
}



////////////////////////////////////////////////
// 子战斗力类型定义
enum SubFightScoreType {
  MIN = 0,

  ACTOR_BASE = 0,                     // 角色基础属性
  SOUL,                               // 英灵图鉴解锁属性
  SOUL_GROUP,                         // 英灵套卡属性
  GUILD_SKILL,                        // 军团技能
  GEM,                                // 宝石
  SPIRIT,                             // 精灵
  NOBLE,                              // 贵族
  ELEMENT,                            // 元素
  SWORD,                              // 魔剑
  LEGENDARY_WEAPON,                   // 名武
  TASK_LAYER,                         // 纹章
  AXE_EQUIPMENT,                      // 战士装备
  SHOOT_EQUIPMENT,                    // 弓手装备
  MAGIC_EQUIPMENT,                    // 法师装备
  AXE_RUNE,                           // 战士符文
  SHOOT_RUNE,                         // 弓手符文
  MAGIC_RUNE,                         // 法师符文

  MAX,
}

