include "game_request.thrift"
include "task.thrift"

namespace cpp game.server.request
namespace py ccrequest.task


// 请求发任务
// type: REQUEST_TASK_ACCEPT 
struct RequestTaskAccept {
  1 : required i32 task_id_;        // 请求发送任务
  2 : required i64 actor_id_;       // 玩家ID
  3 : required task.TaskType type_; // 任务类型
}

// 请求任务中有没有该采集NPC
// type: REQUEST_TASK_CHECK_GATHER
struct RequestTaskCheckGather {
  1 : required i32 npc_template_;   // NPC模板ID
  2 : required i64 actor_id_;       // 玩家ID
}

// 请求最大的纹章ID
// type: REQUEST_TASK_MAX_LAYER
struct RequestTaskMaxLayer {
  1 : required i64 actor_id_;       // 玩家ID
  2 : required i32 task_layer_;     // 纹章id
}

// 获取任务条件所在的位置坐标
// type: REQUEST_TASK_CONDITION_COORDINATE
struct RequestTaskConditionCoordinate {
  1 : required i64 actor_id_;       // 玩家ID
  2 : required task.TaskType type_; // 任务类型                                 (IN)
  3 : required i32 task_id_;        // 任务ID(对于非主线任务，就是任务标识)     (IN)
  4 : required i32 condition_pos_;  // 任务中条件位置                           (IN)
  5 : required i32 map_id_;         // 条件所在的地图                           (OUT)
  6 : required i32 x_;              // 条件所在的X坐标                          (OUT)
  7 : required i32 y_;              // 条件所在的Y坐标                          (OUT)
}

enum RequestTaskOptType  {
  ACCEPT,
  FINISH,
}

// 获取接取任务的位置
// type: REQUEST_TASK_ACCEPT_AND_FINISH_COORDINATE
struct RequestTaskAcceptAndFinishCoordinate {           
  1 : required i64 actor_id_;                // 玩家ID
  2 : required task.TaskType type_;          // 任务类型                               (IN)
  3 : required i32 task_id_;                 // 任务ID(对于非主线任务，就是任务标识)   (IN)
  4 : required RequestTaskOptType opt_type_; // 获取的类型                             (IN)
  5 : required i32 map_id_;                  // 任务所在的地图                         (OUT)
  6 : required i32 x_;                       // 任务所在的X坐标                        (OUT)
  7 : required i32 y_;                       // 任务所在的Y坐标                        (OUT)
}
