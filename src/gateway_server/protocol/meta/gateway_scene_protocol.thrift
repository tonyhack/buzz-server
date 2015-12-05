include "actor.thrift"
include "entity.thrift"
include "task.thrift"
include "gateway_protocol.thrift"

namespace cpp gateway.protocol
namespace as3 com.dreamfire.sanguo.data.definition.protocol.gateway

///////////////////////////////////
// 请求跳转场景
// type: MESSAGE_SCENE_REQUEST_JUMP
// dir: client->gateway
struct MessageSceneRequestJump {
  1 : required string npc_id_;                              // 跳转Npc的ID
}



///////////////////////////////////
// 玩家跳转场景
// type: MESSAGE_SCENE_ACTOR_JUMP
// dir: gateway->client
struct MessageSceneActorJump {
  1 : required i32 map_;                                    // 目标地图ID
  2 : required i32 x_;                                      // 地图坐标x
  3 : required i32 y_;                                      // 地图坐标y
}



///////////////////////////////////
// 玩家选择复活方式
// type: MESSAGE_SCENE_ACTOR_CHOOSE_REVIVE_MODE_REQUEST
// dir: client->gateway
struct MessageSceneActorChooseReviveModeRequest {
  1 : required actor.ReviveType revive_type_;               // 选择的复活方式
  2 : optional bool buy_;                                   // 不足时购买复活道具
}


///////////////////////////////////
// 玩家选择复活方式
// type: MESSAGE_SCENE_ACTOR_CHOOSE_REVIVE_MODE_RESPONSE
// dir: client->gateway
struct MessageSceneActorChooseReviveModeResponse {
  1 : required actor.ReviveType revive_type_;               // 选择的复活方式
}


///////////////////////////////////
// 场景昼夜转换
// type: MESSAGE_SCENE_DAYNIGHT_CHANGE
// dir: gateway->client
enum DayNightStatus {
  STATUS_MORNING = 0,
  STATUS_DAYTIME,
  STATUS_EVENING,
  STATUS_NIGHT,
}
struct MessageSceneDayNightChange {
  1 : required DayNightStatus status_;                      // 当前昼夜状态
}



///////////////////////////////////
// 角色被杀死消息
// type: MESSAGE_SCENE_ROLE_KILLED
// dir: gateway->client
struct MessageSceneRoleKilled {
  1 : required entity.EntityType attacker_type_;            // 攻击者类型
  2 : required string attacker_id_;                         // 攻击者id
  3 : optional string attacker_name_;                       // 攻击者名字(类型为玩家有效)
  4 : optional i32 template_id_;                            // 模板ID(类型为NPC时有效)
}



///////////////////////////////////
// 采集
// type: MESSAGE_SCENE_GATHER
// dir: client->gateway
struct MessageSceneGather {
  1 : required string npc_;                                 // NPC采集
}



///////////////////////////////////
// 采集
// type: MESSAGE_SCENE_GATHER_RESPONSE
// dir: gateway->client
struct MessageSceneGatherResponse {
  1 : required string npc_template_;                        // NPC模板id
}



///////////////////////////////////
// 拾取请求
// type: MESSAGE_SCENE_PICK_UP_REQUEST
// dir: client->gateway
struct MessageScenePickUpRequest {
  1 : required string item_;                                // 道具ID
}



///////////////////////////////////
// 拾取回应
// type: MESSAGE_SCENE_PICK_UP_RESPONSE
// dir: gateway->client
struct MessageScenePickUpResponse {
  1 : required bool result_;                                // 结果
}

//////////////////////////////////
//场景跳转类型
enum SceneJumpType {
 WORLD_JUMP = 0,
 TASK_ACCEPT_JUMP,
 TASK_FINISH_JUMP,
 TASK_CONDITION_JIMP,
}

///////////////////////////////////
// 世界地图请求跳转
// type: MESSAGE_SCENE_JUMP_REQUEST
// dir: client->gateway
struct MessageSceneJumpRequest {
  1 : required SceneJumpType type_;                         // 场景跳转类型
  2 : optional i32 id_;                                     // 目标地图ID/任务ID
  3 : optional task.TaskType task_type_;                    // 任务跳转时，任务类型
  4 : optional i32 condition_pos_;                          // 任务跳转时, 任务条件所在的位置
}

//////////////////////////////////
// 世界地图请求跳转返回
// type: MESSAGE_SCENE_JUMP_RESPONSE
// dir: gateway->client
struct MessageSceneJumpRsponse {
  1 : required bool result_;                                  // 结果
  2 : required i32 num_;                                      // 世界跳转剩余次数
}

/////////////////////////////////
// 世界BOSS 信息
struct SceneBossField {
  1 : required i32 template_id_;                            // BOSS模板ID
  2 : required string killer_;                              // 击杀者
  3 : required bool boss_live_;                             // BOSS 是否活着
}

//////////////////////////////////
// 场景数据初始化
// type: MESSAGE_SCENE_WORLD_BOSS_INITIALIZE
// dir: gateway->client
struct MessageSceneWorldBossInitialize {
  1 : required list<SceneBossField> fields_;                // boss 信息列表
}

/////////////////////////////////
// 广播世界BOSS被杀死
// type: MESSAGE_SCENE_BOSS_KILLED
// dir:  gateway->client
struct MessageSceneBossKilled {
  1 : required i32 template_id_;                            // boss 模板ID
  2 : required string killer_;                              // 击杀者
}

////////////////////////////////
// 召唤世界BOSS
// type: MESSAGE_SCENE_SUMMON_WORLD_BOSS
// dir: gateway->client
struct MessageSceneSummonWorldBoss {
  1 : required i32 template_id_;                            // 世界BOSS出现
}

///////////////////////////////
// 请求打坐
// type: MESSAGE_SCENE_ZAZEN_REQUEST
// dir: client->gateway
struct MessageSceneZazenRequest {
}

///////////////////////////////
// 请求打坐响应
// type: MESSAGE_SCENE_ZAZEN_RESPONSE
// dir: gateway->client
struct MessageSceneZazenResponse {
  1 : required bool result_;                                // 请求结果
}

///////////////////////////////
// 请求取消打坐
// type: MESSAGE_SCENE_CANCEL_ZAZEN_REQUEST
// dir: client->gateway
struct MessageSceneCancelZazenRequest {
}

///////////////////////////////
// 请求取消打坐返回
// type: MESSAGE_SCENE_CANCEL_ZAZEN_RESPONSE
// dir: gateway->client
struct MessageSceneCancelZazenResponse {
  1 : required bool result_;
}

///////////////////////////////
// 通知客户端打坐收益
// type: MESSAGE_SCENE_ZAZEN_INCOME_NOTICE
// dir: gateway->client
struct MessageSceneZazenIncomeNotice {
  1 : required i32 exp_;                                  // 加的经验
  2 : required i32 nimbus_;                               // 加的灵气
}

//////////////////////////////
// 通知客户端连斩
// type: MESSAGE_SCENE_CONSECUTIVE_KILL_NOTICE
// dir: gateway->client
struct MessageSceneConsecutiveKillNotice {
  1 : required i32 kill_num_;                             // 击杀怪物数
}

//////////////////////////////
// 分线同步
// type: MESSAGE_SCENE_BRANCH_SYNCHRONIZE
// dir: gateway->client
struct MessageSceneBranchSynchronize {
  1 : required i16 current_branch_;                       // 当前线路
}

//////////////////////////////
// 分线状态请求
// type: MESSAGE_SCENE_BRANCH_STATUS_REQUEST
// dir: client->gateway
struct MessageSceneBranchStatusRequest {
}

//////////////////////////////
// 分线状态回应
// type: MESSAGE_SCENE_BRANCH_STATUS_RESPONSE
// dir: gateway->client
struct MessageSceneBranchStatusResponse {
  1 : required list<i32> actor_number_;                   // 分线人数
}

//////////////////////////////
// 分线切换请求
// type: MESSAGE_SCENE_BRANCH_SWITCH_REQUEST
// dir: client->gateway
struct MessageSceneBranchSwitchRequest {
  1 : required i16 branch_id_;                            // 分线ID
}

//////////////////////////////
// 分线切换回应
// type: MESSAGE_SCENE_BRANCH_SWITCH_RESPONSE
// dir: gateway->client
struct MessageSceneBranchSwitchResponse {
  1 : required bool success_;                             // 是否成功
}

