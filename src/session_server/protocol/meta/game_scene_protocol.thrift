include "session_game_protocol.thrift"
include "entity.thrift"
include "npc.thrift"

namespace cpp session.protocol


////////////////////////////////////////
// 场景登陆请求
// type: MESSAGE_SCENE_LOGIN_REQUEST
// dir: game->session
struct MessageSceneLoginRequest {
  1 : required i64 scene_;                                // 场景ID
  2 : required bool dynamic_;                             // 是否为动态场景
}


////////////////////////////////////////
// 场景登陆回应
// type: MESSAGE_SCENE_LOGIN_RESPONSE
// dir: session->game
struct MessageSceneLoginResponse {
  1 : required i64 scene_;                                // 场景ID
  2 : required bool result_;                              // 是否成功
}


struct SceneFields {
  1 : required i64 scene_id_;                             // 场景ID
  2 : required bool dynamic_;                             // 是否为动态场景
}

////////////////////////////////////////
// 场景批量登陆请求
// type: MESSAGE_SCENE_BATCH_LOGIN_REQUEST
// dir: game->session
struct MessageSceneBatchLoginRequest {
  1 : required list<SceneFields> scenes_;                 // 场景列表
}


////////////////////////////////////////
// 场景批量登陆回应
// type: MESSAGE_SCENE_BATCH_LOGIN_RESPONSE
// dir: session->game
struct MessageSceneBatchLoginResponse {
  1 : required list<i64> successes_;                      // 成功列表
  2 : required list<i64> failures_;                       // 失败列表
}


////////////////////////////////////////
// 场景登出请求
// type: MESSAGE_SCENE_LOGOUT_REQUEST
// dir: game->session
struct MessageSceneLogoutRequest {
  1 : required i64 scene_;                                // 场景ID
}


////////////////////////////////////////
// 场景登出回应
// type: MESSAGE_SCENE_LOGOUT_RESPONSE
// dir: session->game
struct MessageSceneLogoutResponse {
  1 : required i64 scene_;                                // 场景ID
  2 : required bool result_;                              // 是否成功
}


////////////////////////////////////////
// 场景跳转通知
// type: MESSAGE_SCENE_JUMP_NOTICE
// dir: session->game
struct MessageSceneJumpNotice {
  1 : required i64 actor_;                                // 玩家ID
  2 : required i64 scene_;                                // 场景ID
  3 : required i32 x_;                                    // 坐标x
  4 : required i32 y_;                                    // 坐标y
}

////////////////////////////////////////
// 通知召唤世界boss
// type: MESSAGE_SCENE_SUMMON_WORLD_BOSS
// dir: session->game
struct MessageSceneSummonWorldBoss {
  1 : required i64 id_;                                   // 场景ID
  2 : required i32 x_;                                    // 坐标X
  3 : required i32 y_;                                    // 坐标Y
  4 : required i32 width_;                                // 随机宽度
  5 : required i32 heigth_;                               // 随机高度
  6 : required i32 template_id_;                          // 世界BOSS id
}

///////////////////////////////////////
// 世界BOSS被杀死通知session
// type: MESSAGE_SCENE_WORLD_BOSS_KILLED_NOTICE
// dir: game->session
struct MessageSceneWorldBossKilledNotice {
  1 : required string killer_;                            // 杀BOSS玩家名
  2 : required i64 id_;                                   // 场景ID
}


///////////////////////////////////////////////
// 跳转地图请求
// type: MESSAGE_SCENE_ACTOR_JUMP_MAP_REQUEST
// dir: game->session
struct MessageSceneActorJumpMapRequest {
  1 : required i64 id_;                                   // 玩家ID
  2 : required i16 map_;                                  // 地图ID
  3 : required i32 x_;                                    // 坐标x
  4 : required i32 y_;                                    // 坐标y
}


///////////////////////////////////////////////
// 跳转地图回应
// type: MESSAGE_SCENE_ACTOR_JUMP_MAP_RESPONSE
// dir: session->game
struct MessageSceneActorJumpMapResponse {
  1 : required i64 id_;                                   // 玩家ID
  2 : required i16 map_;                                  // 地图ID
  3 : required i64 scene_;                                // 场景ID
  4 : required i32 x_;                                    // 坐标x
  5 : required i32 y_;                                    // 坐标y
  6 : required bool game_cross_;                          // 是否跨游戏服
}


///////////////////////////////////////////////
// 跳转场景请求
// type: MESSAGE_SCENE_JUMP_SCENE_REQUEST
// dir: game->session
struct MessageSceneJumpSceneRequest {
  1 : required entity.EntityType type_;                   // 类型
  2 : required i64 id_;                                   // ID
  3 : required i64 scene_;                                // 场景ID
  4 : required i32 x_;                                    // 坐标x
  5 : required i32 y_;                                    // 坐标y
  6 : optional npc.NpcSnapshotField npc_snapshot_;        // npc快照数据
}


///////////////////////////////////////////////
// 跳转场景回应
// type: MESSAGE_SCENE_JUMP_SCENE_RESPONSE
// dir: session->game
struct MessageSceneJumpSceneResponse {
  1 : required entity.EntityType type_;                   // 类型
  2 : required i64 id_;                                   // ID
  3 : required i64 scene_;                                // 场景ID
  4 : required i32 x_;                                    // 坐标x
  5 : required i32 y_;                                    // 坐标y
}


///////////////////////////////////////////////
// 玩家跳转登入请求到会话
// type: MESSAGE_SCENE_ACTOR_JUMP_LOGIN_REQUEST
// dir: game->session
struct MessageSceneActorJumpLoginRequest {
  1 : required i64 id_;                                   // 玩家ID
  2 : required i64 scene_;                                // 场景ID
  3 : required i32 x_;                                    // 坐标x
  4 : required i32 y_;                                    // 坐标y
}


///////////////////////////////////////////////
// 玩家跳转登出请求到会话
// type: MESSAGE_SCENE_ACTOR_JUMP_LOGOUT_REQUEST
// dir: game->session
struct MessageSceneActorJumpLogoutRequest {
  1 : required i64 id_;                                   // 玩家ID
  2 : required i64 scene_;                                // 场景ID
  3 : required i32 x_;                                    // 坐标x
  4 : required i32 y_;                                    // 坐标y
}


///////////////////////////////////////////////
// npc跳转同步
// type: MESSAGE_SCENE_NPC_JUMP_SYNCHRONIZE
// dir: session->game
struct MessageSceneNpcJumpSynchronize {
  1 : required i64 scene_;                                // 场景ID
  2 : required i32 x_;                                    // 坐标x
  3 : required i32 y_;                                    // 坐标y
  4 : required npc.NpcSnapshotField npc_snapshot_;        // npc快照数据
}


///////////////////////////////////////////////
// 地图开线同步
// type: MESSAGE_SCENE_BRANCH_MAP_SYNCHRONIZE
// dir: session->game
struct MessageSceneBranchMapSynchronize {
  1 : required i16 map_;                                  // 地图ID
  2 : required i16 branch_;                               // 分线ID
}


///////////////////////////////////////////////
// 跳转地图通知
// type: MESSAGE_SCENE_ACTOR_SWITCH_BRANCH_NOTICE
// dir: session->game
struct MessageSceneActorSwitchBranchNotice {
  1 : required i64 actor_;                                // 玩家ID
  2 : required i64 scene_;                                // 场景ID
}

