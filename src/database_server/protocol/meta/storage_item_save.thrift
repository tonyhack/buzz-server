include "storage_item_login.thrift"

namespace cpp database.protocol

// type: STORAGE_ITEM_SAVE
struct StorageItemSaveRequest {
  1 : required i64 id_;                                             // 玩家ID
  2 : required byte contain_type_                                   // 容器类型
  3 : optional list<storage_item_login.StorageItemField> fields_;   // 字段表
}

struct StorageItemSaveResponse {
  1 : required bool result_;
}

