include "storage_social_actor_login.thrift"

namespace cpp database.protocol

// type STORAGE_SOCIAL_ACTOR_SAVE

struct StorageSocialActorSaveRequest {
  1 : required i64 actor_id_;                                             // 玩家ID
  2 : required storage_social_actor_login.StorageSocialActorField field_; // 社交玩家字段
}

struct StorageSocialActorSaveResponse {
  1 : required bool result_;                                              // 是否成功
}

