include "storage_soul_actor_login.thrift"

namespace cpp database.protocol

// type: STORAGE_SOUL_ACTOR_SAVE
struct StorageSoulActorSaveRequest {
  1 : required i64 actor_id_;                                           // 玩家ID
  2 : required storage_soul_actor_login.StorageSoulActorField field_;   // 字段
}

struct StorageSoulActorSaveResponse {
  1 : required bool result_;
}

