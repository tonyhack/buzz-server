namespace cpp entity
namespace py ccentity.actor
namespace as3 com.dreamfire.sanguo.data.definition.entity

enum ActorAoiFields {
  AOI_BEGIN = 0,

  GENDER = 0,                                   // 性别
  APPEARANCE,                                   // 外貌(0:头像, 1:发型)
  BROWN_NAME,                                   // 褐名
  CURRENT_SWORD,                                // 魔剑当前阶级
  CURRENT_SPIRIT_FACADE,                        // 精灵当前外观
  VIP,                                          // VIP

  AOI_END,
}

enum ActorClientFields {
  CLIENT_BEGIN = 0,

  CURRENT_EXP = 0,                              // 当前经验

  NIMBUS,                                       // 灵气

  GOLD,                                         // 金币
  FREEDOM_DOLLARS,                              // 非绑币
  RESTRICT_DOLLARS,                             // 绑定币

  HONOUR,                                       // 荣誉
  WAR_SOUL,                                     // 战魂(神威)
  GEM_SHARD,                                    // 宝石碎片

  PACKET_CAPACITY,                              // 包裹容量
  STORAGE_CAPACITY,                             // 仓库容量

  PK_MODE,                                      // PK模式

  FCM_TYPE,                                     // 防沉迷类型
  FCM_STATUS,                                   // 防沉迷状态

  JUSTICE,                                      // 正义点

  MIRACLE_INTEGRAL,                             // 奇迹积分
  LEGEND_INTEGRAL,                              // 传奇积分
  ARENA_INTEGRAL,                               // 竞技场积分

  CLIENT_END,
}

enum ActorServerFields {
  SERVER_BEGIN = 0,
  
  KILL_MOB_AND_ZAZEN_EXP_ADD_PERCENT = 0,       // 打怪和冥想经验加成比(万分之)

  KILL_MOB_AND_ZAZEN_NIMBUS_ADD_PERCENT,        // 打怪和冥想灵气加成比(万分之)

  SERVER_END,
}

struct ActorAoiField {
  1 : required ActorAoiFields field_;
  2 : required i32 value_;
}

struct ActorClientField {
  1 : required ActorClientFields field_;
  2 : required i32 value_;
}

enum ActorPKModeType {
  PEACE = 0,                                  // 和平模式
  ATTACK = 1,                                 // 攻击模式
  BATTLE = 2,                                 // 战场模式
}

enum ActorPKChooseType {
  SELF_DEFENSE = 0x1,                         // 自卫反击
  NOT_ATTACK_TEAMMATE = 0x2,                  // 不攻击同队玩家
  NOT_ATTACK_SAME_GUILD = 0x4,                // 不攻击同公会
  NOT_ATTACK_NO_BROWN_ACTOR = 0x8,            // 不攻击非褐名玩家
}

// 复活方式
enum ReviveType { 
  SPAWN_POINT = 0,                            // 复活点
  STAY = 1,                                   // 原地
}

enum FcmType {
  UNKNOWN = 0,                                // 未进行防沉迷认证
  MINOR = 1,                                  // 已进行防沉迷认证并且未成年
  ADULT = 2,                                  // 已进行防沉迷认证并且已成年
}

enum FcmStatusType {
  NORMAL = 0,                                 // 正常状态
  HALF_INCOME = 1,                            // 收益减半
  ZERO_INCOME = 2,                            // 零收益
}

