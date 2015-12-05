namespace cpp database.protocol

enum WorldDataField {
  ID = 0,
  DATA,
  
  MAX,
}

const list<string> kWorldDataFieldStr = [
  "id",                                            // ID
  "data",                                          // 数据
]

struct StorageWorldDataField {
  1 : required i64 id_;
  2 : required string data_;
}

// type: STORAGE_WORLD_DATA_SAVE

struct StorageWorldDataSaveRequest {
  1 : required StorageWorldDataField field_;       // 世界数据
}

struct StorageWorldDataSaveResponse {
  1 : required bool result_;                       // 是否成功
}

