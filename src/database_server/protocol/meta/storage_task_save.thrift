include "storage_task_login.thrift"

namespace cpp database.protocol

// type: STORAGE_TASK_SAVE
struct StorageTaskSaveRequest {
  1 : required i64 id_;                                                 // 玩家ID
  2 : required list<storage_task_login.StorageTaskField> actives_;      // 活动任务集
  3 : required list<storage_task_login.StorageTaskFinish> finishes_;    // 完成任务集
}

struct StorageTaskSaveResponse {
  1 : required bool result_;                                            // 是否成功
}

