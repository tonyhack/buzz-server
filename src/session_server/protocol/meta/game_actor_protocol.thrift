include "gender.thrift"
include "functionality.thrift"
include "vip.thrift"
include "vocation.thrift"
include "session_game_protocol.thrift"

namespace cpp session.protocol


///////////////////////////////////////////////
// 玩家登陆请求
// type: MESSAGE_ACTOR_LOGIN_REQUEST
// dir: session->game
struct GameMessageActorLoginRequest {
  1 : required i64 actor_id_;                         // 玩家ID
  2 : required i32 gateway_;                          // 所在网关ID
  3 : required bool jump_scene_;                      // 是否是跳转场景
  4 : optional i64 scene_;                            // 场景ID
  5 : optional i32 x_;                                // 坐标X
  6 : optional i32 y_;                                // 坐标Y
  7 : optional binary data_;                          // 基本数据
}


///////////////////////////////////////////////
// 玩家登陆回应
// type: MESSAGE_ACTOR_LOGIN_RESPONSE
// dir: game->session
struct GameMessageActorLoginResponse {
  1 : required bool logined_;
  2 : required i64 actor_id_;
  3 : required string actor_name_;
  4 : required bool jump_scene_;                      // 是否是跳转场景
}


///////////////////////////////////////////////
// 玩家退出请求
// type: MESSAGE_ACTOR_LOGOUT_REQUEST
// dir: session->game, game->session
struct GameMessageActorLogout {
  1 : required i64 actor_id_;
}


///////////////////////////////////////////////
// 玩家批量登陆请求
// type: MESSAGE_ACTOR_BATCH_LOGIN_REQUEST
// dir: game->session
struct SessionActorInfo {
  1 : required i64 id_;                               // 玩家ID
  2 : required string name_;                          // 玩家名称
  3 : required i32 level_;                            // 玩家等级
  4 : required vocation.VocationType vocation_;       // 职业
  5 : required gender.GenderType gender_;             // 性别
  6 : required i32 map_;                              // 地图ID
  7 : required string functionality_state_;           // 系统功能开启状态
  8 : required i32 axe_fight_score_;                  // 战士战斗力
  9 : required i32 shoot_fight_score_;                // 弓手战斗力
  10: required i32 magic_fight_score_;                // 法师战斗力
  11: required i64 last_logout_time_;                 // 上次下线时间
  12: required i32 gateway_;                          // 所在网关
  13: required vip.VipType vip_type_;                 // vip 类型
  14: required i64 vip_expire_time_;                  // vip 剩余时间
}
struct MessageActorBatchLoginRequest {
  1 : required list<SessionActorInfo> actors_;        // 登陆玩家列表
}


///////////////////////////////////////////////
// 玩家等级同步
// type: MESSAGE_ACTOR_LEVEL_SYNCHRONIZE
// dir: game->session
struct MessageActorLevelSynchronize {
  1 : required i64 actor_id_;                         // 玩家ID
  2 : required i32 old_level_;                        // 原等级
  3 : required i32 new_level_;                        // 新等级
}


///////////////////////////////////////////////
// 玩家职业同步
// type: MESSAGE_ACTOR_VOCATION_SYNCHRONIZE
// dir: game->session
struct MessageActorVocationSynchronzie {
  1 : required i64 actor_id_;                         // 玩家ID
  2 : required vocation.VocationType src_vocation_;   // 原职业
  3 : required vocation.VocationType dst_vocation_;   // 新职业
}


///////////////////////////////////////////////
// 玩家系统功能开启状态同步
// type: MESSAGE_ACTOR_FUNCTIONALITY_STATE_SYCHRONIZE
// dir: game->session
struct MessageActorFunctionalityStateSynchronize {
  1 : required i64 actor_id_;                         // 玩家ID
  2 : required functionality.FunctionalityType type_; // 功能ID
  3 : required bool state_;                           // 开启状态
}


///////////////////////////////////////////////
// 玩家战斗力同步
// type: MESSAGE_ACTOR_FIGHT_SCORE_SYCHRONIZE
// dir: game->session
struct MessageActorFightScoreSychronize {
  1 : required i64 actor_id_;                         // 玩家ID
  2 : optional i32 axe_fight_score_;                  // 战士战斗力
  3 : optional i32 shoot_fight_score_;                // 弓手战斗力
  4 : optional i32 magic_fight_score_;                // 法师战斗力
}


////////////////////////////////////////////////
// 玩家对比请求
// type: MESSAGE_ACTOR_COMPARE_REQUEST
// dir: game->session
struct MessageActorCompareRequest {
  1 : required i64 actor_;                            // 玩家ID
  2 : required i64 compare_actor_;                    // 被比较的玩家ID
}


////////////////////////////////////////////////
// 玩家对比同步
// type: MESSAGE_ACTOR_COMPARE_SYNCHRONIZE
// dir: session->game
struct MessageActorCompareSynchronize {
  1 : required i64 actor_;                            // 玩家ID
  2 : required i64 compare_actor_;                    // 被比较的玩家ID
}


////////////////////////////////////////////////
// 玩家外观同步
// type: MESSAGE_ACTOR_FACADE_SYNCHRONIZE
// dir: session->game
struct MessageActorFacadeSynchronize {
  1 : required i64 actor_;                            // 玩家ID
  2 : required i64 observe_actor_;                    // 被观察玩家ID
}


////////////////////////////////////////////////
// 玩家VIP信息
// type: MESSAGE_ACTOR_VIP_SYNCHRONIZE
struct MessageActorVipSynchronize {
  1 : required i64 actor_;                            // 玩家ID
  2 : required vip.VipType vip_type_;                 // VIP同步信息
  3 : required i64 viP_expire_time_;                  // vip 到期时间
}
