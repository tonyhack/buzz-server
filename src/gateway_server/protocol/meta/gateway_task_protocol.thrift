include "gateway_protocol.thrift"
include "task.thrift"

namespace cpp gateway.protocol
namespace as3 com.dreamfire.sanguo.data.definition.protocol.gateway


/////////////////////////////////////////
// 任务数据
struct TaskVar {
  1 : required i32 type_;                               // 变量类型
  2 : required i32 id_;                                 // 变量ID
  3 : required i32 value_;                              // 变量值
}

struct TaskFields {
  1 : required i32 id_;                                 // 任务ID
  2 : required list<TaskVar> variables_;                // 任务变量
}

/////////////////////////////////////////
// 任务数据初始化
// type: MESSAGE_TASK_INITIALIZE
// dir: gateway->client
struct MessageTaskInitialize {
  1 : required list<TaskFields> tasks_;                 // 正在进行的任务
  2 : required list<i32> finish_tasks_;                 // 已经完成的任务
}


/////////////////////////////////////////
// 任务数据同步
// type: MESSAGE_TASK_SYNCHRONIZE
// dir: gateway->client
struct MessageTaskSynchronize {
  1 : required TaskFields task_;                        // 任务数据
  2 : required task.TaskType task_type_;                // 任务类型
}


/////////////////////////////////////////
// 接受一个任务
// type: MESSAGE_TASK_ACCEPT
// dir: client->gateway, gateway->client
struct MessageTaskAccept {
  1 : required i32 id_;                                 // 任务ID
  2 : required string npc_;                             // 接受任务的NPC
  3 : required task.TaskType task_type_;                // 任务类型
}


/////////////////////////////////////////
// 完成一个任务
// type: MESSAGE_TASK_FINISH
// dir: client->gateway, gateway->client
struct MessageTaskFinish {
  1 : required i32 id_;                                 // 任务ID
  2 : required string npc_;                             // 接受任务的NPC
  3 : required byte reward_id_;                         // 奖励ID(多选一的奖励，从0到4)
  4 : required task.TaskType task_type_;                // 任务类型
}


/////////////////////////////////////////
// 放弃一个任务
// type: MESSAGE_TASK_ABANDON
// dir: client->gateway, gateway->client
struct MessageTaskAbandon {
  1 : required i32 id_;                                 // 任务ID
  2 : required task.TaskType task_type_;                // 任务类型
}


/////////////////////////////////////////
// 与NPC对话
// type: MESSAGE_TASK_TALK_NPC
// dir: client->gateway, gateway->client
struct MessageTaskTalkNpc {
  1 : required string npc_;                             // 对话npc的id
  2 : required i32 task_id_;                            // 任务ID
}

/////////////////////////////////////////
// 召唤NPC任务
// type: MESSAGE_TASK_SUMMON_NPC_REQUEST
// dir: client->gateway
struct MessageTaskSummonNpcRequest {
  1 : required i32 npc_;                                // 召唤NPC的id
  2 : required string accept_npc_;                      // 接受任务的NPC
}


////////////////////////////////////////
// 召唤NPC任务返回
// type: MESSAGE_TASK_SUMMON_NPC_RESPONSE
// dir:  gateway->client
struct MessageTaskSummonNpcResponse {
  1 : required i32 npc_;                              // 召唤NPC id
}


////////////////////////////////////////
// 循环任务初始化
// type: MESSAGE_TASK_CIRCLE_INITIALIZE
// dir: gateway->client
struct MessageTaskCircleInitialize {
  1 : required i32 current_round_;                     // 当前进行中得轮数
  2 : required i32 total_round_;                       // 当前总共轮数
  3 : required i32 circle_condition_id_;               // 当前进行中的环信息
  4 : required i32 level_stage_;                       // 玩家等级段
  5 : required i32 current_circle_;                    // 当前的环数
  6 : required TaskFields task_;                       // 当前的任务
  7 : required task.TaskCircleRoundState state_;       // 轮状态
}

////////////////////////////////////////
// 循环任务操作类型
// type: MESSAGE_TASK_CIRCLE_OPT_REQUEST
// dir: client->gateway
struct MessageTaskCircleOptRequest {
  1 : required task.TaskCircleRefulshType type_;        // 玩家操作类型
}

///////////////////////////////////////
// 循环任务响应客户端操作请求
// type:MESSAGE_TASK_CIRCLE_OPT_RESPONSE
// dir: gateway->client
struct MessageTaskCircleOptResponse {
  1 : required task.TaskCircleRefulshType type_;        // 玩家操作类型
  2 : required i32 result_;                             // 返回结果
}

///////////////////////////////////////
// 奇遇任务初始化
// type:MESSAGE_TASK_TRIP_INITIALIZE
// dir: gateway->client
struct MessageTaskTripInitialize {
  1 : required i32 task_num_;                           // 奇遇任务剩余数量
  2 : required list<TaskFields> tasks_;                 // 奇遇任务当前的任务
}

///////////////////////////////////////
// 奇遇任务操作类型
// type:MESSAGE_TASK_TRIP_OPT_REQUEST
// dir: client->gateway
struct MessageTaskTripOptRequest {
  1 : required task.TaskTripOptType type_;              // 奇遇任务操作类型
  2 : required i32 value_;                              // 操作类型值
}

//////////////////////////////////////
// 奇遇任务操作类型
// type:MESSAGE_TASK_TRIP_OPT_RESPONSE
// dir:gateway->client
struct MessageTaskTripResponse {
  1 : required task.TaskTripOptType type_;              // 奇遇任务操作类型
  2 : required i32 result_;                             // 操作类型结果 
}

///////////////////////////////////////
// 每日任务初始化
// type:MESSAGE_TASK_DAILY_INITIALIZE
// dir: gateway->client
struct MessageTaskDailyInitialize {
  1 : required list<TaskFields> tasks_;                 // 每日任务当前的任务
  2 : required list<i32> finish_tasks_;                 // 完成任务
  3 : required i32 daily_exploit_;                      // 每日的军功
  4 : required list<bool> exploit_awards_;              // 军功奖励
}

//////////////////////////////////////
// 请求每日完成/接取任务
// type: MESSAGE_TASK_DAILY__REQUEST
// dir: client->gateway
struct MessageTaskDailyRequest {                        
  1 : required i32 id_;                                 // 任务ID
  2 : required i32 opt_;                                // 0 :接取 1:完成 2: 快速完成
}

// 请求完成任务响应
// type: MESSAGE_TASK_DAILY_RESPONSE
// dir: gateway->client
struct MessageTaskDailyResponse {
  1 : required i32 id_;                                 // 任务ID(0:失败)
  2 : required i32 opt_;                                // 0 :接取 1:完成 2: 快速完成
}

//////////////////////////////////////
// 请求奖励
// type: MESSAGE_TASK_DAILY_AWARD_REQUEST
// dir: client->gateway
struct MessageTaskDailyAwardRequest {
  1 : required i32 id_;                                // 请求奖励(0)
}

/////////////////////////////////////
// 请求奖励返回
// type: MESSAGE_TASK_DAILY_AWARD_RESPONSE
// dir: gateway->client
struct MessageTaskDaliyAwardResponse {
  1 : required i32 id_;                                  // 返回
}


/////////////////////////////////////////
// 护送任务初始化
// type: MESSAGE_TASK_TRANSPORT_INITIALIZE
// dir: gateway->client
struct MessageTaskTransportInitialize {
  1 : required i32 finished_count_;                     // 已完成次数
  2 : required i32 free_refreshed_count_;               // 免费的已刷新次数
  3 : required i32 robed_count_;                        // 已抢劫次数
  4 : required i32 quality_;                            // 护送品质
  5 : required task.TaskTransportStatus status_;        // 护送状态
}

/////////////////////////////////////////
// 护送数据同步
// type: MESSAGE_TASK_TRANSPORT_SYNCHRONIZE
// dir: gateway->client
struct MessageTaskTransportSynchronize {
  1 : optional i32 finished_count_;                     // 已完成次数
  2 : optional i32 refreshed_count_;                    // 免费的已刷新次数
  3 : optional i32 robed_count_;                        // 已抢劫次数
  4 : optional i32 transport_quality_;                  // 护送品质
  5 : optional task.TaskTransportStatus status_;        // 护送状态
}


/////////////////////////////////////////
// 护送刷新请求
// type: MESSAGE_TASK_TRANSPORT_REFRESH_QUALITY_REQUEST
// dir: client->gateway
struct MessageTaskTransportRefreshQualityRequest {
}

/////////////////////////////////////////
// 护送刷新回应
// type: MESSAGE_TASK_TRANSPORT_REFRESH_QUALITY_RESPONSE
// dir: gateway->client
struct MessageTaskTransportRefreshQualityResponse {
  1 : required bool result_;                            // 结果
}


/////////////////////////////////////////
// 护送任务接受请求
// type: MESSAGE_TASK_TRANSPORT_ACCEPT_REQUEST
// dir: client->gateway
struct MessageTaskTransportAcceptRequest {
  1 : required string npc_;                             // NPC的ID
}

/////////////////////////////////////////
// 护送任务接受回应
// type: MESSAGE_TASK_TRANSPORT_ACCEPT_RESPONSE
// dir: gateway->client
struct MessageTaskTransportAcceptResponse {
  1 : required bool result_;                            // 结果
}


/////////////////////////////////////////
// 护送任务完成请求
// type: MESSAGE_TASK_TRANSPORT_FINISH_REQUEST
// dir: client->gateway
struct MessageTaskTransportFinishRequest {
  1 : required string npc_;                             // NPC的ID
  2 : required bool force_success_;                     // 是否强制成功
}

/////////////////////////////////////////
// type: MESSAGE_TASK_TRANSPORT_FINISH_RESPONSE
// dir: gateway->client
struct MessageTaskTransportFinishResponse {
  1 : required bool result_;                            // 结果
}

