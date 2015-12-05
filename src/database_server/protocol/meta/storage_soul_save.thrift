include "storage_soul_login.thrift"

namespace cpp database.protocol

// type: STORAGE_SOUL_SAVE
struct StorageSoulSaveRequest {
  1 : required i64 id_;                                               // 玩家ID
  2 : required list<storage_soul_login.StorageSoulField> fields_;     // 字段表
}

struct StorageSoulSaveResponse {
  1 : required bool result_;
}

