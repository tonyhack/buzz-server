include "storage_playing_actor_login.thrift"

namespace cpp database.protocol

// type STORAGE_PLAYING_ACTOR_SAVE

struct StoragePlayingActorSaveRequest {
  1 : required i64 actor_id_;                                                         // 玩家ID
  2 : required storage_playing_actor_login.StoragePlayingActorField field_;           // 副本模块字段
}

struct StoragePlayingActorSaveResponse {
  1 : required bool result_;                                                          // 是否成功
}

