namespace cpp database.protocol

enum GemField {
  LOCATION_TYPE = 0,
  LOCATION_INDEX,
  GEM_ID,
  GEM_LEVEL,
  GEM_EXP,
  LOCKED,

  MAX,
}

const list<string> kGemFieldStr = [
  "location_type",                                 // 宝石位置类型
  "location_index",                                // 宝石位置索引
  "gem_id",                                        // 宝石ID
  "gem_level",                                     // 宝石等级
  "gem_exp",                                       // 宝石经验
  "locked",                                        // 是否锁定
]

enum GemLoginResult {
  SUCCESS = 0,
  ERROR_UNKNOWN,
}

struct StorageGemField {
  1 : required i32 location_type_;
  2 : required i16 location_index_;
  3 : required i32 gem_id_;
  4 : required i32 gem_level_;
  5 : required i32 gem_exp_;
  6 : required i32 locked_;
}

// type: STORAGE_GEM_LOGIN

struct StorageGemLoginRequest {
  1 : required i64 actor_id_;                      // 角色ID
}

struct StorageGemLoginResponse {
  1 : required GemLoginResult result_;             // 结果
  2 : required list<StorageGemField> fields_       // 结果集
}

