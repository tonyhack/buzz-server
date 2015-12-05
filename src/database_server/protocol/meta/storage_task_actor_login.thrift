namespace cpp database.protocol

enum TaskActorField {
  CIRCLE_FLAG,
  ROUND_NUM,
  TOTAL_ROUND,
  CIRCLE_NUM,
  LV_STAGE,
  CONDITION_ID,
  TRIP_NUM_,
  EXPLOIT_AWARD,
  DAILY_EXPLIOT,
  TRANSPORT_FINISHED_COUNT,
  TRANSPORT_FREE_REFRESHED_COUNT,
  TRANSPORT_ROBED_COUNT,
  TRANSPORT_CURRENT_TRANSPORT,
  TRANSPORT_STATUS,

  MAX,
}

const list<string> kTaskActorFieldStr = [
  "circle_flag",                                    // 循环任务标记
  "round_num",                                      // 任务当前的轮
  "total_round",                                    // 总轮数
  "circle_num",                                     // 当前环
  "lv_stage",                                       // 当前环的等级段
  "condition_id",                                   // 当前环的条件ID
  "trip_num",                                       // 奇遇任务当前做过数量
  "exploit_award",                                  // 军功奖励列表
  "daily_exploit",                                  // 每日军功
  "transport_finished_count",                       // 护送完成次数
  "transport_free_refreshed_count",                 // 护送免费刷新次数
  "transport_robed_count",                          // 护送抢劫次数
  "transport_current_transport",                    // 护送当前ID
  "transport_status",                               // 护送状态
]

enum TaskActorResult {
  SUCCESS = 0,
  ERROR_UNKNOWN,
}

struct StorageTaskActorField {
  1 : required byte flag_;
  2 : required i32 round_num_;
  3 : required i32 total_num_;
  4 : required i32 circle_num_;
  5 : required i32 lv_stage_;
  6 : required i32 condition_id_;
  7 : required i32 trip_num_;
  8 : required i32 exploit_award_;
  9 : required i32 daily_exploit_;
  10: required i32 transport_finished_count_;
  11: required i32 transport_free_refreshed_count_;
  12: required i32 transport_robed_count_;
  13: required i32 transport_current_transport_;
  14: required i32 transport_status_;
}


// type: STORAGE_TASK_ACTOR_LOGIN

struct StorageTaskActorLoginRequest {
  1 : required i64 actor_id_;                       // 玩家ID
}

struct StorageTaskActorLoginResponse {
  1 : required TaskActorResult result_;             // 结果
  2 : required StorageTaskActorField data_;         // 玩家任务数据
}

