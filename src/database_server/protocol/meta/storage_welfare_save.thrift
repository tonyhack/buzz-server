include "storage_welfare_load.thrift"

namespace cpp database.protocol

// type: STORAGE_WELFARE_SAVE
struct StorageWelfareSave {
  1 : required i64 actor_;
  2 : required storage_welfare_load.StorageWelfareField field_;
}

