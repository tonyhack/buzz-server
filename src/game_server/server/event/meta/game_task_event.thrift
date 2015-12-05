include "task.thrift"

namespace cpp game.server.event
namespace py ccevent.task

// 任务完成事件(如果是主线任务和奇遇任务,)
// type: EVENT_TASK_FINISH
struct EventTaskFinish {
  1 : required i64 actor_id_;           // 玩家id
  2 : required i32 task_id_;            // 完成任务id
  3 : required task.TaskType type_;     // 任务类型
}


// 接取任务事件(主线任务和奇遇任务)
// type: EVENT_TASK_ACCEPT
struct EventTaskAccept {
  1 : required i64 actor_id_;           // 玩家id
  2 : required i32 task_id_;            // 完成任务id
  3 : required task.TaskType type_;     // 任务类型
}
