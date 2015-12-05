include "storage_buff_login.thrift"

namespace cpp database.protocol

// type: STORAGE_buff_SAVE
struct StorageBuffSaveRequest {
  1 : required i64 id_;
  2 : optional list<storage_buff_login.StorageBuffField> fields_;
}

struct StorageBuffSaveResponse {
  1 : required bool result_;
}

