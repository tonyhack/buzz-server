namespace cpp entity
namespace py ccentity.achieve
namespace as3 com.dreamfire.sanguo.data.definition.entity

////////////////////////////////////////////////
// 成就类型定义
enum AchieveConditionType {
  MIN = 0,
  SOUL_UNLOCK = 1,                                  // 解锁X个Y类型的英灵（Y可以是英灵类型，也可以是具体的某个英灵）
  SOUL_UPGRAGE_LEVEL,                               // 将X个英灵升到Y级
  SKILL_USE,                                        // 使用特定技能
  FUNCTIONALITY_OPEN,                               // 开启指定功能
  SPIRIT_UPGRADE_LEVEL,                             // 将精灵升到X级
  SPIRIT_COLLECT_SHAPE,                             // 收集X个指定类型的精灵外形
  KILL_ROLE,                                        // 通过指定方法击杀X个特定对象
  BAG_EXPAND,                                       // 背包扩容
  KILLED_SELF,                                      // 因特定原因导致死亡X次
  EQUIP_STRENGTH,                                   // 准备强化
  WEAR_EQUIP,                                       // 穿装备
  GEM_OPT,                                          // 星魂操作
  GEM_STRENGTH,                                     // 星魂强化
  RELIVE_WAY,                                       // 复活方式
  VOCATION_SET,                                     // 职业设定
  SOUL_SCHEME,                                      // 英灵设置
  SWORD_UPGRADE,                                    // 魔剑升阶
  SWORD_STRENGTH,                                   // 魔剑强化                              
  HOMELAND_BUILDING_UPGRADE,                        // 建筑升级
  HOMELAND_REVENUE,                                 // 家园收益
  HOMELAND_USE,                                     // 家园收益次数
  HOMELAND_WATER,                                   // 家园浇水
  HOMELAND_OPT,                                     // 家园操作
  HOMELAND_ATTACK_NUM,                              // 家园进攻次数
  CORPS_OPT,                                        // 军团基础行为
  CORPS_JOB,                                        // 担任军团职位
  CORPS_GET_HOLY_CITY,                              // 拥有圣城
  TO_SENCE,                                         // 到达场景次数         
  NOLIBITY_GET_LEVEL,                               // 当前爵位
  USE_ITEM,                                         // 使用道具
  RESOURCE,                                         // 获取/消费资源
  ACTOR_GET_VALUE,                                  // 获取玩家属性值
  PASS_STORY_COPY,                                  // 通关剧情战役
  SELF_KILLED_SKILL,                                // 自己被技能杀死
  KILLED_SELF_ACTOR_FIGHTING,                       // 自己被特定战斗力
  KILLED_SELF_ACTOR_LEVEL,                          // 自己被特定等级玩家
  KILLED_SELF_ACTOR_APPEAR,                         // 自己被特定幻化玩家
  KILL_ACTOR_FIGHTING,                              // 杀死特定玩家
  KILL_ACTOR_LEVEL,                                 // 杀死特定玩家
  KILL_ACTOR_APPEAR,                                // 杀死特定玩家
  KILL_ACTOR_SEX,                                   // 杀死特定玩家
  VIP_STATUS,                                       // VIP 状态
  MAX,
}


//////////////////////////////////////////////////
// 使用特定技能
enum AchieveSkillUseType {
  ID = 0,                         // 技能ID
  TYPE,                           // 技能类型
}

/////////////////////////////////////////////////
// 击杀对象
enum AchieveKillRoleType {
  ANY_WAY =0,
  APPEAR_STATUS = 1,
  BEAST_STATUS = 2,
  WING_STATUS = 3,
}

////////////////////////////////////////////////
// 被某个实体杀死
enum AchieveKilledSelfType {
  ACTOR = 0,
  ANY_WAY = 1,
  ANY_NPC = 2,
  SPECIALLY_NPC = 3,
}

////////////////////////////////////////////////
// 装备强化
enum AchieveEquipStrengthType {
  SUCCESS = 0,
  FAILED = 1,
}

///////////////////////////////////////////////
// 拥有特定的装备
enum AchieveWearEquipType {
  QUALITY = 0,
  STRENGTH_LEVEL = 1,
}

///////////////////////////////////////////////
// 星辰操作
enum AchieveGemOptType {
  INLAY = 0,
  DOWN = 1,
}

///////////////////////////////////////////////
// 复活方式
enum AchieveReliveType {
  CITY = 0,
  SLOT = 1,
}

///////////////////////////////////////////////
// 家园收益
enum AchieveHomelandRenenueType {
  GOLDSTONE = 0,
  IMPOSE = 1,
}

///////////////////////////////////////////////
// 家园使用次数
enum AchieveHomelandUseType {
  GOLDSTONE = 0,
  IMPOSE = 1,
}

//////////////////////////////////////////////
// 家园浇水
enum AchieveHomelandWaterType {
  SELF = 0,
  OTHER = 1,
}

////////////////////////////////////////////
// 家园操作
enum AchieveHomelandOptType {
  TREE = 0,
  VISIT_HOMELAND = 1,
  ASSITANCE = 2,
}

//////////////////////////////////////////////
// 家园进攻次数
enum AchieveAttackType {
  SUM = 0,
  DAY = 1,
}

//////////////////////////////////////////////
// 军团行为
enum AchieveCopsActionType {
  ENTER = 0,
  LEAVE = 1,
  CREATE = 2,
}

/////////////////////////////////////////////
// 积累花费
enum AchieveResourceType {
  SPEND = 0,
  GET = 1,
}

////////////////////////////////////////////
// 角色属性
enum AchieveActorAttrType {
  LEVEL = 0,
  FIGHTING = 1,
}

////////////////////////////////////////////
// 通关剧情战役
enum AchievePassTalkFightingType {
  PASS = 0,

}

////////////////////////////////////////////
// 自己被特定玩家杀死
enum AchieveSelfKilledType {
  FIGHTING = 0,
  LEVEL = 1,
  APPEAR_STATUS =2,
}

///////////////////////////////////////////
// 杀死特定玩家
enum AchieveKillActorType {
  FIGHTING = 0,
  LEVEL = 1,
  APPEAR_STATUS = 2,
  SEX = 3,
}
