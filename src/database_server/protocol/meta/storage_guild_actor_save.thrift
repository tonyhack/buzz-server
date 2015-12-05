include "storage_guild_actor_login.thrift"

namespace cpp database.protocol

// type STORAGE_GUILD_ACTOR_SAVE

struct StorageGuildActorSaveRequest {
  1 : required i64 actor_id_;                                           // 玩家ID
  2 : required storage_guild_actor_login.StorageGuildActorField field_; // 公会玩家字段
}

struct StorageGuildActorSaveResponse {
  1 : required bool result_;                                            // 是否成功
}

