include "storage_skill_login.thrift"

namespace cpp database.protocol

// type: STORAGE_SKILL_SAVE
struct StorageSkillSaveRequest {
  1 : required i64 id_;
  2 : required i32 skillid_;
  3 : required i16 opration_type_; //1 add , 2 update 3 del
  4 : required i16 skill_level_;
}

struct StorageSkillSaveResponse {
  1 : required bool result_;
}

