namespace cpp database.protocol

enum GuildActorField {
  GUILD_ID = 0,
  GUILD_POSITION,
  CURRENT_CONTRIBUTION_VALUE,
  TOTAL_CONTRIBUTION_VALUE,
  GOLD_CONTRIBUTION,
  FREEDOM_DOLLARS_CONTRIBUTION,
  LIGHT_CRYSTAL_CONTRIBUTION,
  DARK_CRYSTAL_CONTRIBUTION,
  HOLY_CRYSTAL_CONTRIBUTION,
  GUILD_SKILLS,
  GUILD_BUFFS,
  AWARDED_GUILD_PLAYINGS,

  MAX,
}

const list<string> kGuildActorFieldStr = [
  "guild_id",                                      // 公会ID(若玩家无公会, 则为上一次加入公会的ID)
  "guild_position",                                // 公会职位
  "current_contribution_value",                    // 当前贡献值
  "total_contribution_value",                      // 总贡献值
  "gold_contribution",                             // 金币捐赠量
  "freedom_dollars_contribution",                  // 钻石捐赠量
  "light_crystal_contribution",                    // 光晶石捐赠量
  "dark_crystal_contribution",                     // 暗晶石捐赠量
  "holy_crystal_contribution",                     // 圣晶石捐赠量
  "guild_skills",                                  // 公会技能
  "guild_buffs",                                   // 公会BUFF
  "awarded_guild_playings",                        // 已领取过奖励的公会副本
]

enum GuildActorLoginResult {
  SUCCESS = 0,
  NOT_FOUND = 1,
  ERROR_UNKNOWN,

  MAX,
}

struct StorageGuildActorField {
  1 : required i64 guild_id_;
  2 : required i32 guild_position_;
  3 : required i32 current_contribution_value_;
  4 : required i32 total_contribution_value_;
  5 : required i32 gold_contribution_;
  6 : required i32 freedom_dollars_contribution_;
  7 : required i32 light_crystal_contribution_;
  8 : required i32 dark_crystal_contribution_;
  9 : required i32 holy_crystal_contribution_;
  10: required string guild_skills_;
  11: required string guild_buffs_;
  12: required string awarded_guild_playings_;
}

// type STORAGE_GUILD_ACTOR_LOGIN

struct StorageGuildActorLoginRequest {
  1 : required i64 actor_id_;                      // 玩家ID
}

struct StorageGuildActorLoginResponse {
  1 : required GuildActorLoginResult result_;      // 结果
  2 : required StorageGuildActorField field_;      // 公会玩家字段
}

