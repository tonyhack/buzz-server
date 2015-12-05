include "storage_actor_login.thrift"

namespace cpp database.protocol


// type: STORAGE_ACTOR_SAVE
struct StorageActorSaveRequest {
  1 : required i64 id_;
  2 : required storage_actor_login.StorageActorField fields_;
}

struct StorageActorSaveResponse {
  1 : required bool result_;
}

