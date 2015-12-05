namespace cpp database.protocol

// type: STORAGE_ACTOR_ONLINE
struct StorageActorOnlineRequest {
  1 : required i64 id_;           // 角色ID
  2 : required i64 sequence_;     // 请求序列号
}

struct StorageActorOnlineResponse {
  1 : required byte online_;      // 在线状态
}

