namespace cpp database.protocol

enum SocialActorField {
  SIGNATURE,
  FRIENDS,
  BLACKLIST,
  ENEMIES,
  HOMELAND_UPGRADE_START_TIME,
  HOMELAND_UPGRADE_FINISH_TIME,
  HOMELAND_TEMPLE_LEVEL,
  HOMELAND_TEMPLE_HARVEST_TIMES,
  HOMELAND_TEMPLE_FORCE_HARVEST_TIMES,
  HOMELAND_TEMPLE_NEXT_HARVEST_TIME,
  HOMELAND_GOLDMINE_LEVEL,
  HOMELAND_GOLDMINE_ROBBED_TIMES,
  HOMELAND_GOLDMINE_LOSS_RATE,
  HOMELAND_GOLDMINE_NEXT_HARVEST_TIME,
  HOMELAND_GOLDMINE_ROB_TIMES,
  HOMELAND_GOLDMINE_NEXT_ROB_TIME,
  HOMELAND_POOL_LEVEL,
  HOMELAND_POOL_LAST_HARVEST_TIME,
  HOMELAND_CHURCH_FREE_PRAY_TIMES,
  HOMELAND_CHURCH_PAID_PRAY_TIMES,
  HOMELAND_CHURCH_FREE_REFRESH_TIMES,
  HOMELAND_CHURCH_CHEST_TYPE,
  HOMELAND_TREE_LEVEL,
  HOMELAND_TREE_HARVEST_TIMES,
  HOMELAND_TREE_STAGE,
  HOMELAND_TREE_WATERED_TIMES,
  HOMELAND_TREE_NEXT_WATER_TIME,
  HOMELAND_TREE_GROWING_FINISH_TIME,
  HOMELAND_TREE_ASSIST_WATER_TIMES,
  HOMELAND_TREE_WATERED_ACTORS,
  HOMELAND_TREE_STOLEN_ACTORS,
  HOMELAND_TOWER1_LEVEL,
  HOMELAND_TOWER2_LEVEL,
  HOMELAND_STRONGHOLD_LEVEL,
  HOMELAND_STRONGHOLD_NEXT_CALL_SUPPORT_TIME,
  HOMELAND_STRONGHOLD_SUPPORTED_SOULS,
  HOMELAND_STRONGHOLD_SUPPORT_SOULS,
  HOMELAND_STRONGHOLD_SUPPORT_TIMES,
  HOMELAND_INVADERS,
  HOMELAND_EVENTS,
  LAST_SAVE_TIME,
  SOUL_DATA,

  MAX,
}

const list<string> kSocialActorFieldStr = [
  "signature",                                     // 签名
  "friends",                                       // 好友
  "blacklist",                                     // 黑名单
  "enemies",                                       // 仇人
  "homeland_upgrade_start_time"                    // 家园建筑升级开始时间
  "homeland_upgrade_finish_time",                  // 家园建筑升级完成时间
  "homeland_temple_level",                         // 家园神殿等级
  "homeland_temple_harvest_times",                 // 家园神殿征收次数
  "homeland_temple_force_harvest_times",           // 家园神殿强征次数
  "homeland_temple_next_harvest_time",             // 家园神殿下次征收时间
  "homeland_goldmine_level",                       // 家园金矿等级
  "homeland_goldmine_robbed_times",                // 家园金矿被掠夺次数
  "homeland_goldmine_loss_rate",                   // 家园金矿损失比例
  "homeland_goldmine_next_harvest_time",           // 家园金矿下次收获时间
  "homeland_goldmine_rob_times",                   // 家园金矿掠夺次数
  "homeland_goldmine_next_rob_time",               // 家园金矿下次掠夺时间
  "homeland_pool_level",                           // 家园神秘之源等级
  "homeland_pool_last_harvest_time",               // 家园神秘之源上次领取时间
  "homeland_church_free_pray_times",               // 家园祈福堂免费祈福次数
  "homeland_church_paid_pray_times",               // 家园祈福堂钻石祈福次数
  "homeland_church_free_refresh_times"             // 家园祈福堂免费刷新次数
  "homeland_church_chest_type",                    // 家园祈福堂宝箱类型
  "homeland_tree_level",                           // 家园神树等级
  "homeland_tree_harvest_times",                   // 家园神树收获次数
  "homeland_tree_stage",                           // 家园神树所处的阶段
  "homeland_tree_watered_times",                   // 家园神树完成度
  "homeland_tree_next_water_time",                 // 家园神树下次浇水时间
  "homeland_tree_growing_finish_time",             // 家园神树生长完成时间
  "homeland_tree_assist_water_times",              // 家园神树帮助好友浇水次数
  "homeland_tree_watered_actors",                  // 家园神树帮助浇水的好友
  "homeland_tree_stolen_actors",                   // 家园神树抢收的好友
  "homeland_tower1_level",                         // 家园炮塔1等级
  "homeland_tower2_level",                         // 家园炮塔2等级
  "homeland_stronghold_level",                     // 家园军团据点等级
  "homeland_stronghold_next_call_support_time",    // 家园军团下次申请支援时间
  "homeland_stronghold_supported_souls",           // 家园军团据点被支援的英灵
  "homeland_stronghold_support_souls",             // 家园军团据点支援的英灵
  "homeland_stronghold_support_times",             // 家园军团据点支援次数
  "homeland_invaders",                             // 家园侵入者
  "homeland_events",                               // 家园告示板
  "last_save_time",                                // 上次保存时间
  "soul_data",                                     // 英灵数据
]

enum SocialActorLoginResult {
  SUCCESS = 0,
  ERROR_UNKNOWN,

  MAX,
}

struct StorageSocialActorField {
  1 : required string signature_;
  2 : required string friends_;
  3 : required string blacklist_;
  4 : required string enemies_;
  5 : required i64 homeland_upgrade_start_time_;
  6 : required i64 homeland_upgrade_finish_time_;
  7 : required i32 homeland_temple_level_;
  8 : required i32 homeland_temple_harvest_times_;
  9 : required i32 homeland_temple_force_harvest_times_;
  10: required i64 homeland_temple_next_harvest_time_;
  11: required i32 homeland_goldmine_level_;
  12: required i32 homeland_goldmine_robbed_times_;
  13: required i32 homeland_goldmine_loss_rate_;
  14: required i64 homeland_goldmine_next_harvest_time_;
  15: required i32 homeland_goldmine_rob_times_;
  16: required i64 homeland_goldmine_next_rob_time_;
  17: required i32 homeland_pool_level_;
  18: required i64 homeland_pool_last_harvest_time_;
  19: required i32 homeland_church_free_pray_times_;
  20: required i32 homeland_church_paid_pray_times_;
  21: required i32 homeland_church_free_refresh_times_;
  22: required i32 homeland_church_chest_type_;
  23: required i32 homeland_tree_level_;
  24: required i32 homeland_tree_harvest_times_; 
  25: required i32 homeland_tree_stage_;
  26: required i32 homeland_tree_watered_times_;
  27: required i64 homeland_tree_next_water_time_;
  28: required i64 homeland_tree_growing_finish_time_; 
  29: required i32 homeland_tree_assist_water_times_;
  30: required string homeland_tree_watered_actors_;
  31: required string homeland_tree_stolen_actors_;
  32: required i32 homeland_tower1_level_;
  33: required i32 homeland_tower2_level_;
  34: required i32 homeland_stronghold_level_;
  35: required i64 homeland_stronghold_next_call_support_time_;
  36: required string homeland_stronghold_supported_souls_;
  37: required string homeland_stronghold_support_souls_;
  38: required i32 homeland_stronghold_support_times_;
  39: required string homeland_invaders_;
  40: required string homeland_events_;
  41: required i64 last_save_time_;
  42: required string soul_data_;
}

// type STORAGE_SOCIAL_ACTOR_LOGIN

struct StorageSocialActorLoginRequest {
  1 : required i64 actor_id_;                      // 玩家ID
  2 : optional bool offline_load_;                 // 是否是离线加载
  3 : optional i64 offline_task_id_;               // 离线任务ID
}

struct StorageSocialActorLoginResponse {
  1 : required SocialActorLoginResult result_;     // 结果
  2 : required StorageSocialActorField field_;     // 社交玩家字段
}

