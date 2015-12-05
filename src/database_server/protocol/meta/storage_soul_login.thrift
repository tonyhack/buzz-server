namespace cpp database.protocol

enum SoulField {
  ID = 0,
  LEVEL,
  STEP,
  COMMON_SKILL_LEVEL,
  APPEAR_SKILL_LEVEL,
  SOUL_SKILL_LEVEL,

  MAX,
}

const list<string> kSoulFieldStr = [
  "id",                                         // 英灵ID
  "level",                                      // 等级
  "step",                                       // 阶
  "common_skill_level",                         // 普通技能等级
  "appear_skill_level",                         // 幻化技能等级
  "soul_skill_level",                           // 英灵技能等级
]

enum SoulLoginResult {
  SUCCESS = 0,
  ERROR_UNKNOWN,
}

struct StorageSoulField {
  1 : required i32 id_;
  2 : required i32 level_;
  3 : required i32 step_;
  4 : required i32 common_skill_level_;
  5 : required i32 appear_skill_level_;
  6 : required i32 soul_skill_level_;
}


// type: STORAGE_SOUL_LOGIN

struct StorageSoulLoginRequest {
  1 : required i64 actor_id_;                   // 玩家ID
}

struct StorageSoulLoginResponse {
  1 : required SoulLoginResult result_;         // 结果
  2 : required list<StorageSoulField> fields_;  // 结果集
}

