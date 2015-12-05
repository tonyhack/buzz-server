include "storage_gem_login.thrift"

namespace cpp database.protocol

// type: STORAGE_GEM_SAVE

struct StorageGemSaveRequest {
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : required list<storage_gem_login.StorageGemField> fields_; // 字段表
}

struct StorageGemSaveResponse {
  1 : required bool result_;                                    // 是否成功
}

