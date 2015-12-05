include "storage_item_actor_login.thrift"

namespace cpp database.protocol

// type STORAGE_ITEM_ACTOR_SAVE

struct StorageItemActorSaveRequest {
  1 : required i64 actor_id_;                                         // 玩家ID
  2 : required storage_item_actor_login.StorageItemActorField field_; // 道具模块字段
}

struct StorageItemActorSaveResponse {
  1 : required bool result_;                                          // 是否成功
}

