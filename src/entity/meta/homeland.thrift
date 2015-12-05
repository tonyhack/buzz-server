namespace cpp entity
namespace py ccentity.homeland
namespace as3 com.dreamfire.sanguo.data.definition.entity

////////////////////////////////////////////////
// 家园神树阶段类型定义
enum HomelandTreeStageType {
  MIN = 0,

  WATERING = 0,                       // 浇水阶段
  GROWING,                            // 生长阶段
  HARVEST,                            // 收获阶段

  MAX,
}



////////////////////////////////////////////////
// 家园告示板信息类型定义
enum HomelandEventType {
  MIN = 0,

  HOMELAND_VISIT = 0,                 // 家园拜访
  CHURCH_PRAY,                        // 祈福堂祈福
  TREE_ASSIST_WATER,                  // 帮助好友神树浇水
  TREE_ASSIST_WATERED,                // 神树被好友帮助浇水
  TREE_STEAL,                         // 抢收好友神树
  TREE_STOLEN,                        // 神树被好友抢收
  STRONGHOLD_SUPPORT,                 // 军团据点支援
  STRONGHOLD_SUPPORTED,               // 军团据点被支援
  BATTLE_DEFENCE_SUCCESS,             // 攻防战防守成功 
  BATTLE_DEFENCE_FAILED,              // 攻防战防守失败
  BATTLE_ATTACK_SUCCESS,              // 攻防战进攻成功
  BATTLE_ATTACK_FAILED,               // 攻防战进攻失败

  MAX,
}



////////////////////////////////////////////////
// 家园攻防战势力类型
enum HomelandBattleSideType {
  MIN  = 0,

  ATTACK = 0,                         // 进攻方
  DEFENCE,                            // 防守方

  MAX,
}



////////////////////////////////////////////////
// 家园攻防战单位类型
enum HomelandBattleUnitType {
  MIN = 0,

  SOUL = 0,                          // 英灵
  ACTOR,                             // 主角
  TOWER,                             // 炮塔

  MAX,
}



////////////////////////////////////////////////
// 家园攻防战单位行动类型
enum HomelandBattleActionType {
  MIN = 0,

  ATTACK = 0,                        // 普通攻击
  SPECIAL_SKILL_AXE,                 // 战士特殊技能
  SPECIAL_SKILL_SHOOT,               // 弓手特殊技能
  SPECIAL_SKILL_MAGIC,               // 法师特殊技能
  AOE_ACTOR,                         // 主角AOE攻击
  AOE_TOWER,                         // 炮塔AOE攻击

  MAX,
}



////////////////////////////////////////////////
// 家园攻防战单位状态变化类型
enum HomelandBattleStateChangeType {
  MIN = 0,
  
  HURT = 0,                          // 受到伤害
  DODGE,                             // 闪避
  CRIT,                              // 被暴击
  DEAD,                              // 死亡

  MAX,
}



////////////////////////////////////////////////
// 家园攻防战结果类型
enum HomelandBattleResultType {
  MIN = 0,

  LOSE = 0,                          // 战败
  TINY_WIN,                          // 小胜
  GREAT_WIN,                         // 大胜
  PERFECT_WIN,                       // 完胜

  MAX,
}

