include "storage_spirit_facade_login.thrift"

namespace cpp database.protocol

// type: STORAGE_SPIRIT_FACADE_SAVE

struct StorageSpiritFacadeSaveRequest {
  1 : required i64 actor_id_;                                                      // 玩家ID
  2 : required list<storage_spirit_facade_login.StorageSpiritFacadeField> fields_; // 字段表
}

struct StorageSpiritFacadeSaveResponse {
  1 : required bool result_;                                                       // 是否成功
}

