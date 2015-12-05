namespace cpp database.protocol

enum ActorField {
  NAME = 0,                                 // 姓名
  LEVEL,                                    // 等级
  VOCATION,                                 // 职业
  GENDER,                                   // 性别
  HEAD,                                     // 头像
  HAIR,                                     // 发型
  CURRENT_HP,                               // 血量
  CURRENT_MP,                               // 魔法值
  CURRENT_EXP,                              // 经验
  NIMBUS,                                   // 灵气
  GOLD,                                     // 金币
  FREEDOM_DOLLARS,                          // 非绑金钱
  RESTRICT_DOLLARS,                         // 绑定金钱
  HONOUR,                                   // 荣誉
  WAR_SOUL,                                 // 战魂
  GEM_SHARD,                                // 宝石碎片
  JUSTICE,                                  // 正义点
  MIRACLE_INTEGRAL,                         // 奇迹积分
  LEGEND_INTEGRAL,                          // 传奇积分
  ARENA_INTEGRAL,                           // 竞技场积分
  MAP,                                      // 地图ID
  X,                                        // 坐标X
  Y,                                        // 坐标Y
  PRE_MAP,                                  // 上个地图ID
  PRE_X,                                    // 上个坐标x
  PRE_Y,                                    // 上个坐标y
  BROWN_NAME,                               // 褐名
  PK_MODE,                                  // pk模式
  FUNCTIONALITY_STATE,                      // 系统功能开启状态
  PACKET_CAPACITY,                          // 背包容量
  STORAGE_CAPACITY,                         // 仓库容量
  AXE_FIGHT_SCORE,                          // 战士战斗力
  SHOOT_FIGHT_SCORE,                        // 弓手战斗力
  MAGIC_FIGHT_SCORE,                        // 法师战斗力
  LAST_LOGOUT_TIME,                         // 上次下线时间
  VIP_REMAINDER_TIME,                       // vip剩余时间
  VIP_TYPE,                                 // vip类型 
  WORLD_JUMP_VIP_NUM,                       // 世界地图跳转VIP 次数
  FCM_TYPE,                                 // 防沉迷类型
  FCM_ONLINE_TIME,                          // 防沉迷累计在线时间
  CREATE_TIME,                              // 创建时间
  HP_TASLIMAN,                              // 血护符值
  MP_TASLIMAN,                              // 蓝护符值
  MAX,
}

const list<string> kActorFieldStr = [
  "@ret_name",
  "@ret_level",
  "@ret_vocation",
  "@ret_gender",
  "@ret_head",
  "@ret_hair",
  "@ret_current_hp",
  "@ret_current_mp",
  "@ret_current_exp",
  "@ret_nimbus",
  "@ret_gold"
  "@ret_freedom_dollars",
  "@ret_restrict_dollars",
  "@ret_honour",
  "@ret_war_soul",
  "@ret_gem_shard",
  "@ret_justice",
  "@ret_miracle_integral",
  "@ret_legend_integral",
  "@ret_arena_integral",
  "@ret_map",
  "@ret_x",
  "@ret_y",
  "@ret_pre_map",
  "@ret_pre_x",
  "@ret_pre_y",
  "@ret_brown_name",
  "@ret_pk_mode",
  "@ret_functionality_state",
  "@ret_packet_capacity",
  "@ret_storage_capacity",
  "@ret_axe_fight_score",
  "@ret_shoot_fight_score",
  "@ret_magic_fight_score",
  "@ret_last_logout_time",
  "@ret_vip_remainder_time",
  "@ret_vip_type",
  "@ret_world_jump_vip_num",
  "@ret_fcm_type",
  "@ret_fcm_online_time",
  "@ret_create_time",
  "@ret_hp_tasliman",
  "@ret_mp_tasliman",
]

enum ActorLoginResult {
  SUCCESS = 0,
  ERROR_INEXIST,
  ERROR_UNKNOWN,

  MAX,
}

struct StorageActorField {
  1 : required string name_;                // 角色名
  2 : required i32 level_;                  // 等级
  3 : required i32 vocation_;               // 职业
  4 : required byte gender_;                // 性别
  5 : required byte head_;                  // 头像
  6 : required byte hair_;                  // 发型
  7 : required i32 current_hp_;             // 血量
  8 : required i32 current_mp_;             // 魔法值
  9 : required i32 current_exp_;            // 当前经验
  10: required i32 nimbus_;                 // 灵气
  11: required i32 gold_;                   // 金币
  12: required i32 freedom_dollars_;        // 非绑定金钱
  13: required i32 restrict_dollars_;       // 绑定金钱
  14: required i32 honour_;                 // 荣誉
  15: required i32 war_soul_;               // 战魂
  16: required i32 gem_shard_;              // 宝石碎片
  17: required i32 justice_;                // 正义点
  18: required i32 miracle_integral_;       // 奇迹积分
  19: required i32 legend_integral_;        // 传奇积分
  20: required i32 arena_integral_;         // 竞技场积分
  21: required i16 map_;                    // 所在地图
  22: required i32 x_;                      // 坐标X
  23: required i32 y_;                      // 坐标Y
  24: required i16 pre_map_;                // 所在地图
  25: required i32 pre_x_;                  // 坐标X
  26: required i32 pre_y_;                  // 坐标Y
  27: required i32 brown_name_;             // 褐名
  28: required i32 pk_mode_;                // pk模式
  29: required string functionality_state_; // 系统功能开启状态
  30: required i32 packet_capacity_;        // 背包容量
  31: required i32 storage_capacity_;       // 仓库容量
  32: required i32 axe_fight_score_;        // 战士战斗力
  33: required i32 shoot_fight_score_;      // 弓手战斗力
  34: required i32 magic_fight_score_;      // 法师战斗力
  35: required i64 last_logout_time_;       // 上次下线时间
  36: required i32 vip_remainder_time_;     // VIP剩余时间
  37: required byte vip_type_;              // VIP卡类型
  38: required i32 world_jump_vip_num_;     // 世界地图跳转VIP 次数
  39: required i32 fcm_type_;               // 防沉迷类型
  40: required i32 fcm_online_time_;        // 防沉迷累计在线时间
  41: required i64 create_time_;            // 账号创建时间
  42: required i32 hp_tasliman_;            // 血护符值
  43: required i32 mp_tasliman_;            // 蓝护符值
}

// type: STORAGE_ACTOR_LOGIN
struct StorageActorLoginRequest {
  1 : required i64 id_;                     // 角色ID
  2 : required i32 gateway_;                // 玩家所在网关ID
  3 : optional i64 jump_scene_;             // 地图ID
  4 : optional i32 jump_x_;                 // 坐标X
  5 : optional i32 jump_y_;                 // 坐标Y
}
struct StorageActorLoginResponse {
  1 : required ActorLoginResult result_;    // 结果
  2 : optional StorageActorField field_;    // 角色属性字段
}

