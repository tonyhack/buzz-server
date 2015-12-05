include "storage_cooling_login.thrift"

namespace cpp database.protocol

// type: STORAGE_ACTOR_SAVE
struct StorageCoolingSaveRequest {
  1 : required i64 id_;
  2 : optional list<storage_cooling_login.StorageCoolingField> fields_;
}

struct StorageCoolingSaveResponse {
  1 : required bool result_;
}

