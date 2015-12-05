namespace cpp entity
namespace py ccentity.inform
namespace as3 com.dreamfire.sanguo.data.definition.entity

/////////////////////////////////////////////////////////
// 公告类型定义
//
// NOTE: 模块顺序按照 gateway_protocol.thrift 中的顺序
//
enum InformType {
  MIN = 0,

  ///////////////////////////////
  // 技能公告
  SKILL_KILL_ACTOR,
  SKILL_KILLED_BY_ACTOR,
  SKILL_KILLED_BY_NPC,
  ///////////////////////////////

  ///////////////////////////////
  // 道具公告
  GEM_GET_ORANGE_GEM,
  GEM_HIGH_LEVEL,
  GEM_TOP_LEVEL,
  EQUIPMENT_INTENSIFY_HIGH_LEVEL,
  EQUIPMENT_INTENSIFY_TOP_LEVEL,
  EQUIPMENT_UPGRADE,
  EQUIPMENT_REFINE,
  ///////////////////////////////

  ///////////////////////////////
  // 英灵公告
  SOUL_TOP_LEVEL,
  ///////////////////////////////

  ///////////////////////////////
  // 任务公告
  TASK_CIRCLE_QUICK_FINISH_ROUND,             // 循环任务一键完成整轮任务
  ///////////////////////////////

  ///////////////////////////////
  // 属性公告
  ATTR_NOBLE_ENABLE_FUNC,
  ATTR_NOBLE_UPGRADE_LEVEL,
  ATTR_NOBLE_TOP_LEVEL,
  ATTR_ELEMENT_UPGRADE,
  ATTR_ELEMENT_TOP_LEVEL,
  ATTR_SPIRIT_HIGH_LEVEL,
  ATTR_SPIRIT_TOP_LEVEL,
  ATTR_SWORD_ENABLE_FUNC,
  ATTR_SWORD_UPGRADE_LEVEL,
  ATTR_SWORD_TOP_LEVEL,
  ///////////////////////////////

  ///////////////////////////////
  // 副本公告
  PLAYING_MEMBER_PERIOD_COUNT_ERROR,          // 队员通关次数错误
  PLAYING_MEMBER_LEVEL_ERROR,                 // 队员等级错误
  PLAYING_MEMBER_NUMBER_ERROR,                // 队员数量错误
  PLAYING_MEMBER_PLAYING_ERROR,               // 队员副本中错误
  ///////////////////////////////

  ///////////////////////////////
  // 公会公告
  GUILD_CREATE,
  GUILD_REFUSE_INVITE,
  GUILD_REFUSE_JOIN,
  GUILD_JOIN,
  GUILD_LEAVE,
  GUILD_SET_MEMBER_POSITION,
  GUILD_RESET_MEMBER_POSITION,
  GUILD_GIVE_UP_POSITION,
  GUILD_GIVE_UP_LEADER_POSITION,
  GUILD_UPGRADE,
  GUILD_SHOP_UPGRADE,
  GUILD_COLLEGE_UPGRADE,
  GUILD_BARRACK_UPGRADE,
  GUILD_DONATE,
  GUILD_HIGH_LEVEL,
  ///////////////////////////////

  ///////////////////////////////
  // 玩家公告
  ACTOR_ALTER_VIP_LEVEL3,                   // 玩家VIP等级提升到帝王 
  ///////////////////////////////

  ///////////////////////////////
  // 场景公告
  SCENE_BOSS_SUMMON,                        // 召唤boss公告
  ///////////////////////////////

  MAX,
}

