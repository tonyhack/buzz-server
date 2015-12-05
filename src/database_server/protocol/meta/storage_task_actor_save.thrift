namespace cpp database.protocol
include "storage_task_actor_login.thrift"

enum TaskActorLoginResult {
  SUCCESS = 0,
  ERROR_UNKNOWN,
}

// type: STORAGE_TASK_ACTOR_SAVE

struct StorageTaskActorSaveRequest {
  1 : required i64 actor_id_;                       // 玩家ID
  2 : required storage_task_actor_login.StorageTaskActorField data_;         // 玩家任务数据
}


struct StorageTaskActorSaveResponse {
  1 : required storage_task_actor_login.TaskActorResult result_;             // 结果
}
