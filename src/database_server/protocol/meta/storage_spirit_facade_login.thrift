namespace cpp database.protocol

enum SpiritFacadeField {
  SPIRIT_FACADE_ID = 0,
  EXPIRE_TIME,

  MAX,
}

const list<string> kSpiritFacadeFieldStr = [
  "spirit_facade_id",                                  // 精灵外观ID
  "expire_time",                                       // 失效时间
]

enum SpiritFacadeLoginResult {
  SUCCESS = 0,
  ERROR_UNKNOWN,
}

struct StorageSpiritFacadeField {
  1 : required i32 spirit_facade_id_;
  2 : required i32 expire_time_;
}

// type: STORAGE_SPIRIT_FACADE_LOGIN

struct StorageSpiritFacadeLoginRequest {
  1 : required i64 actor_id_;                          // 角色ID
}

struct StorageSpiritFacadeLoginResponse {
  1 : required SpiritFacadeLoginResult result_;        // 结果
  2 : required list<StorageSpiritFacadeField> fields_; // 结果集
}

