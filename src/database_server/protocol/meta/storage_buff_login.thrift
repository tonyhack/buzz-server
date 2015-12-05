namespace cpp database.protocol

enum BuffField {
  BUFF_ID = 0,
  LEFT_TIME,
  STACK_COUNT,

  MAX,
}

const list<string> kBuffFieldStr = [
  "buff_id",                              // buff ID
  "left_time",                            // buff 剩余时间
  "stack_count",                          // buff 叠加次数
]

enum BuffLoginResult {
  SUCCESS = 0,
  ERROR_UNKNOWN,
}

struct StorageBuffField {
  1 : required i32 buff_id_;
  2 : required i32 left_time_;
  3 : required i32 stack_count_;
}

// type: STORAGE_BUFF_LOGIN
struct StorageBuffLoginRequest {
  1 : required i64 id_;                           // bufID
}
struct StorageBuffLoginResponse {
  1 : required BuffLoginResult result_;        // 结果
  2 : optional list<StorageBuffField> fields_; // 结果集
}

