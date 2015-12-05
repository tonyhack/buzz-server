namespace cpp database.protocol

enum CoolingField {
  COOLING_ID = 0,
  COOLING_TIME,

  MAX,
}

const list<string> kCoolingFieldStr = [
  "cooling_id",                                   // 冷却ID
  "cooling_time"                                  // 剩余冷却时间
]

enum CoolingLoginResult {
  SUCCESS = 0,
  ERROR_UNKNOWN,
}

struct StorageCoolingField {
  1 : required i32 cooling_id_;
  2 : required i64 cooling_time_;
}

// type: STORAGE_COOLING_LOGIN
struct StorageCoolingLoginRequest {
  1 : required i64 id_;                           // 角色ID
}
struct StorageCoolingLoginResponse {
  1 : required CoolingLoginResult result_;        // 结果
  2 : optional list<StorageCoolingField> fields_; // 结果集
}

