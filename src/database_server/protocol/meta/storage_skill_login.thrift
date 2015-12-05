namespace cpp database.protocol

enum SkillField {
  SKILL_ID = 0,
  CURRENT_LEVEL,

  MAX,
}

const list<string> kSkillFieldStr = [
  "skill_id",                                   // 技能ID
  "skill_current_level"                         // 技能当前等级
]

enum SkillLoginResult {
  SUCCESS = 0,
  ERROR_UNKNOWN,
}

struct StorageSkillField {
  1 : required i32 skill_id_;
  2 : required i16 skill_current_level_;
}

// type: STORAGE_SKILL_LOGIN
struct StorageSkillLoginRequest {
  1 : required i64 id_;                           // 角色ID
}
struct StorageSkillLoginResponse {
  1 : required SkillLoginResult result_;        // 结果
  2 : optional list<StorageSkillField> fields_; // 结果集
}

