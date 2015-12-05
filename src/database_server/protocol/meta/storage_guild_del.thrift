namespace cpp database.protocol

// type STORAGE_GUILD_DEL

struct StorageGuildDelRequest {
  1 : required i64 guild_id_;                      // 公会ID
}

struct StorageGuildDelResponse {
  1 : required bool result_;                       // 是否成功
}

