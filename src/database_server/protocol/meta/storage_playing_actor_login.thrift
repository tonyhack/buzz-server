namespace cpp database.protocol

enum PlayingActorField {
  RECORDS,
  RESULTS,
  AUTO_PLAYING,
  AUTO_PLAYING_GROUP,
  AUTO_FINISH_TIME,
  MAZE_AP,
  MAZE_LAST_AP_RECOVER_TIME,
  MAZE_FLOOR,
  MAZE_EXPLORED_AREA,
  MAZE_RECENT_POSITION,
  MAZE_GIFT_NUMBER,

  MAX,
}

const list<string> kPlayingActorFieldStr = [
  "records",                                            // 记录
  "results",                                            // 结果
  "auto_playing",                                       // 自动进行副本
  "auto_playing_group",                                 // 自动进行副本组
  "auto_finish_time",                                   // 自动完成时间
  "maze_ap",                                            // 地宫行动点数
  "maze_last_ap_recover_time",                          // 地宫行动点数上次恢复时间
  "maze_floor",                                         // 地宫层数
  "maze_explored_area",                                 // 地宫已探索区域
  "maze_recent_position",                               // 玩家最近在地宫的位置
  "maze_gift_number",                                   // 剩余未领取地宫通关礼包数量
]

enum PlayingActorLoginResult {
  SUCCESS = 0,
  ERROR_UNKNOWN,

  MAX,
}

struct StoragePlayingActorField {
  1 : required string records_;                         // 记录集
  2 : required string results_;                         // 结果集
  3 : required i32 auto_playing_;                       // 自动进行副本
  4 : required i32 auto_playing_group_;                 // 自动进行副本组
  5 : required i64 auto_finish_time_;                   // 自动完成时间
  6 : required i32 maze_ap_;                            // 地宫行动点数
  7 : required i64 maze_last_ap_recover_time_;          // 地宫行动点数上次恢复时间
  8 : required i32 maze_floor_;                         // 地宫层数
  9 : required string maze_explored_area_;              // 地宫已探索区域
  10: required string maze_recent_position_;            // 玩家最近在地宫的位置
  11: required i32 maze_gift_number_;                   // 剩余未领取地宫通关礼包数量
}


// type STORAGE_PLAYING_ACTOR_LOGIN

struct StoragePlayingActorLoginRequest {
  1 : required i64 actor_id_;                           // 玩家ID
}

struct StoragePlayingActorLoginResponse {
  1 : required PlayingActorLoginResult  result_;        // 结果
  2 : required StoragePlayingActorField field_;         // 副本模块字段
}

