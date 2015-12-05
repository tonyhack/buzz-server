include "storage_attr_actor_login.thrift"

namespace cpp database.protocol

// type STORAGE_ATTR_ACTOR_SAVE

struct StorageAttrActorSaveRequest {
  1 : required i64 actor_id_;                                         // 玩家ID
  2 : required storage_attr_actor_login.StorageAttrActorField field_; // 属性玩家字段
}

struct StorageAttrActorSaveResponse {
  1 : required bool result_;                                          // 是否成功
}

