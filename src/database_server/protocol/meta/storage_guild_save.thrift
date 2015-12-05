namespace cpp database.protocol

enum GuildField {
  ID = 0,
  NAME,
  LOGO,
  LEVEL,
  SUPPRESS_JOIN,
  PENDING_MEMBERS,
  MEMBERS,
  ANNOUNCEMENT,
  EVENTS,
  GOLD,
  LIGHT_CRYSTAL,
  DARK_CRYSTAL,
  HOLY_CRYSTAL,
  LIGHT_ATTRIBUTE,
  DARK_ATTRIBUTE,
  HOLY_ATTRIBUTE,
  SHOP_LEVEL,
  COLLEGE_LEVEL,
  BARRACK_LEVEL,
  LAST_SAVE_TIME,
  OPENED_PLAYING_GROUPS,
  FINISHED_PLAYINGS,
  
  MAX,
}

const list<string> kGuildFieldStr = [
  "id",                                            // 公会ID
  "name",                                          // 公会名
  "logo",                                          // 公会标志
  "level",                                         // 公会等级
  "suppress_join",                                 // 是否关闭申请
  "pending_members",                               // 待加入的成员
  "members",                                       // 公会成员
  "announcement",                                  // 公会公告
  "events",                                        // 公会事件
  "gold",                                          // 公会金币数量
  "light_crystal",                                 // 光晶石数量
  "dark_crystal",                                  // 暗晶石数量
  "holy_crystal",                                  // 圣晶石数量
  "light_attribute",                               // 光属性值
  "dark_attribute",                                // 暗属性值
  "holy_attribute",                                // 圣属性值
  "shop_level",                                    // 商店等级
  "college_level",                                 // 学院等级
  "barrack_level",                                 // 军营等级
  "opened_playing_groups",                         // 解封的公会副本组
  "finished_playings",                             // 完成的公会副本
  "last_save_time",                                // 上次保存时间
]

struct StorageGuildField {
  1 : required i64 id_;
  2 : required string name_;
  3 : required i32 logo_;
  4 : required i32 level_;
  5 : required bool suppress_join_;
  6 : required string pending_members_;
  7 : required string members_;
  8 : required string announcement_;
  9 : required string events_;
  10: required i32 gold_;
  11: required i32 light_crystal_;
  12: required i32 dark_crystal_;
  13: required i32 holy_crystal_;
  14: required i32 light_attribute_;
  15: required i32 dark_attribute_;
  16: required i32 holy_attribute_;
  17: required i32 shop_level_;
  18: required i32 college_level_;
  19: required i32 barrack_level_;
  20: required string opened_playing_groups_;
  21: required string finished_playings_;
  22: required i64 last_save_time_;
}

// type: STORAGE_GUILD_SAVE

struct StorageGuildSaveRequest {
  1 : required StorageGuildField field_;           // 公会数据
}

struct StorageGuildSaveResponse {
  1 : required bool result_;                       // 是否成功
}

