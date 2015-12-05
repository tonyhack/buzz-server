include "gateway_protocol.thrift"

namespace cpp gateway.protocol
namespace as3 com.dreamfire.sanguo.data.definition.protocol.gateway


/////////////////////////////////////////////
// 组队请求
// type:  MESSAGE_TEAM_CREATE_REQUEST
// dir: client->gateway
struct MessageTeamCreateRequest {
}


/////////////////////////////////////////////
// 组队回应
// type: MESSAGE_TEAM_CREATE_RESPONSE
// dir: gateway->client
struct MessageTeamCreateResponse {
  1 : required bool result_;                            // 是否成功
  2 : required string team_;                            // 队伍ID
}


/////////////////////////////////////////////
// 入队请求
// type: MESSAGE_TEAM_JOIN_REQUEST
// dir: client->gateway
struct MessageTeamJoinRequest {
  1 : required string team_;                            // 申请入队的队伍ID
}


/////////////////////////////////////////////
// 入队回应
// type: MESSAGE_TEAM_JOIN_RESPONSE
// dir: gateway->client
struct MessageTeamJoinResponse {
  1 : required bool result_;                            // 申请是否成功发送
}


/////////////////////////////////////////////
// 组队消息同步
// type: MESSAGE_TEAM_JOIN_NOTIFY
// dir: gateway->client
struct MessageTeamJoinNotify {
  1 : required string actor_;                           // 申请者ID
  2 : required string name_;                            // 申请者名称
  3 : required i32 level_;                              // 申请者等级
}


/////////////////////////////////////////////
// 申请入队处理请求
// type: MESSAGE_TEAM_JOIN_DISPOSE_REQUEST
// dir: client->gateway
struct MessageTeamJoinDisposeRequest {
  1 : required string actor_;                           // 申请者玩家ID
  2 : required bool result_;                            // 处理结果(是否同意)
}


/////////////////////////////////////////////
// 申请入队处理回应
// type: MESSAGE_TEAM_JOIN_DISPOSE_RESPONSE
// dir: gateway->client
struct MessageTeamJoinDisposeResponse {
  1 : required bool result_;                            // 处理是否成功发送
}


/////////////////////////////////////////////
// 邀请入队请求
// type: MESSAGE_TEAM_INVITE_REQUEST
// dir: client->gateway
struct MessageTeamInviteRequest {
  1 : optional string actor_;                           // 被邀请者ID
  2 : optional string name_;                            // 被邀请者名称
}


/////////////////////////////////////////////
// 邀请入队回应
// type: MESSAGE_TEAM_INVITE_RESPONSE
// dir: gateway->client
struct MessageTeamInviteResponse {
  1 : required bool result_;                            // 邀请是否成功发送
}


/////////////////////////////////////////////
// 邀请消息同步
// type: MESSAGE_TEAM_INVITE_NOTIFY
// dir: gateway->client
struct MessageTeamInviteNotify {
  1 : required string actor_;                           // 邀请者玩家ID
  2 : required string name_;                            // 邀请者名称
  3 : required i32 level_;                              // 邀请者等级
}


/////////////////////////////////////////////
// 邀请入队处理请求
// type: MESSAGE_TEAM_INVITE_DISPOSE_REQUEST
// dir: client->gateway
struct MessageTeamInviteDisposeRequest {
  1 : required string actor_;                           // 邀请者玩家ID
  2 : required bool result_;                            // 处理结果(是否同意)
}


/////////////////////////////////////////////
// 邀请入队处理回应
// type: MESSAGE_TEAM_INVITE_DISPOSE_RESPONSE
// dir: gateway->client
struct MessageTeamInviteDisposeResponse {
  1 : required bool result_;                            // 处理是否成功发送
}


/////////////////////////////////////////////
// 增加队员
// type: MESSAGE_TEAM_MEMBER_ADD_SYNCHRONIZE
// dir: gateway->client
struct MessageTeamMemberAddSynchronize {
  1 : required string actor_;                           // 队员玩家ID
  2 : required string name_;                            // 队员名称
  3 : required i32 level_;                              // 队员当前等级
  4 : required i32 map_;                                // 队员当前地图ID
}


//////////////////////////////////////////////
// 移除队员
// type: MESSAGE_TEAM_MEMBER_REMOVE_SYNCHRONIZE
// dir: gateway->client
struct MessageTeamMemberRemoveSynchronize {
  1 : required string actor_;                           // 队员玩家ID
}


//////////////////////////////////////////////
// 组队信息同步
// type: MESSAGE_TEAM_SYNCHRONIZE
// dir: gateway->client
// 队员信息
struct TeamMemberFields {
  1 : required string actor_;                           // 玩家ID
  2 : required string name_;                            // 玩家名
  3 : required i32 level_;                              // 当前等级
  4 : required i32 map_;                                // 当前地图
  5 : required i32 x_;                                  // 坐标x
  6 : required i32 y_;                                  // 坐标y
  7 : required i32 current_hp_;                         // 当前生命值
  8 : required i32 max_hp_;                             // 最大生命值
  9 : required i32 current_mp_;                         // 当前魔法值
  10: required i32 max_mp_;                             // 最大魔法值
}
struct MessageTeamSynchronize {
  1 : required string team_id_;                         // 队伍ID
  2 : required string captain_;                         // 队长ID
  3 : optional list<TeamMemberFields> members_;         // 队员列表(不存在时，表示只同步队伍基本信息)
}


//////////////////////////////////////////////
// 队员信息同步
// type: MESSAGE_TEAM_MEMBER_SYNCHRONIZE
// dir: gateway->client
struct MessageTeamMemberSynchronize {
  1 : required string actor_;                           // 玩家ID
  2 : optional string name_;                            // 玩家名
  3 : optional i32 level_;                              // 当前等级
  4 : optional i32 map_;                                // 当前地图
  5 : optional i32 x_;                                  // 坐标x
  6 : optional i32 y_;                                  // 坐标y
  7 : optional i32 current_hp_;                         // 当前生命值
  8 : optional i32 max_hp_;                             // 最大生命值
  9 : optional i32 current_mp_;                         // 当前魔法值
  10: optional i32 max_mp_;                             // 最大魔法值
}


//////////////////////////////////////////////
// 解散队伍请求
// type: MESSAGE_TEAM_DISMISS_REQUEST
// dir: client->gateway
struct MessageTeamDismissRequest {
}


//////////////////////////////////////////////
// 解散队伍回应
// type: MESSAGE_TEAM_DISMISS_RESPONSE
// dir: gateway->client
struct MessageTeamDismissResponse {
  1 : required bool result_;                            // 结果
}


//////////////////////////////////////////////
// 组队解散通知
// type: MESSAGE_TEAM_DISMISS_NOTIFY
// dir: gateway->client
struct MessageTeamDismissNotify {
}


//////////////////////////////////////////////
// 退出请求
// type: MESSAGE_TEAM_QUIT_REQUEST
// dir: client->gateway
struct MessageTeamQuitRequest {
}


//////////////////////////////////////////////
// 退出回应
// type: MESSAGE_TEAM_QUIT_RESPONSE
// dir: gateway->client
struct MessageTeamQuitResponse {
  1 : required bool result_;                            // 结果
}


//////////////////////////////////////////////
// 踢出请求
// type: MESSAGE_TEAM_KICKOUT_REQUEST
// dir: client->gateway
struct MessageTeamKickoutRequest {
  1 : required string actor_;                           // 要踢出的玩家ID
}


//////////////////////////////////////////////
// 踢出回应
// type: MESSAGE_TEAM_KICKOUT_RESPONSE
// dir: gateway->client
struct MessageTeamKickoutResponse {
  1 : required bool result_;                            // 结果
}


//////////////////////////////////////////////
// 设置队长请求
// type: MESSAGE_TEAM_SET_CAPTAIN_REQUEST
// dir: client->gateway
struct MessageTeamSetCaptainRequest {
  1 : required string actor_;                           // 新队长的玩家ID
}


//////////////////////////////////////////////
// 设置队长回应
// type: MESSAGE_TEAM_SET_CAPTAIN_RESPONSE
// dir: gateway->client
struct MessageTeamSetCaptainResponse {
  1 : required bool result_;                            // 结果
}


//////////////////////////////////////////////
// 地图组队信息请求
// type: MESSAGE_TEAM_MAP_TEAMS_REQUEST
// dir: client->gateway
struct MessageTeamMapTeamsRequest {
}


//////////////////////////////////////////////
// 地图组队信息回应
// type: MESSAGE_TEAM_MAP_TEAMS_RESPONSE
// dir: gateway->client
struct TeamFields {
  1 : required string team_;                            // 队伍ID
  2 : required string name_;                            // 队长名
  3 : required byte number_;                            // 队员个数
  4 : required i32 captain_level_;                      // 队长等级
  5 : required i32 average_level_;                      // 平均等级
}
struct MessageTeamMapTeamsResponse {
  1 : required list<TeamFields> teams_;                 // 队伍集合
}


//////////////////////////////////////////////
// 地图玩家信息请求
// type: MESSAGE_TEAM_MAP_ACTORS_REQUEST
// dir: client->gateway
struct MessageTeamMapActorsRequest {
}


//////////////////////////////////////////////
// 地图玩家信息回应
// type: MESSAGE_TEAM_MAP_ACTORS_RESPONSE
// dir: gateway->client
struct TeamActorFields {
  1 : required string actor_;                           // 玩家ID
  2 : required string name_;                            // 玩家名称
  3 : required i32 level_;                              // 玩家等级
  4 : required string team_;                            // 队伍ID
  5 : required string guild_;                           // 公会名
}
struct MessageTeamMapActorsResponse {
  1 : required list<TeamActorFields> actors_;           // 玩家集合
}


//////////////////////////////////////////////
// 队员信息请求
// type: MESSAGE_TEAM_MEMBER_REQUEST
// dir: client->server
struct MessageTeamMemberRequest {
  1 : required string team_;                            // 队伍ID
}


//////////////////////////////////////////////
// 队员信息回应
// type: MESSAGE_TEAM_MEMBER_RESPONSE
// dir: client->gateway
struct MessageTeamMemberResponse {
  1 : required string team_;                            // 队伍ID
  2 : required list<TeamMemberFields> members_;         // 队员信息集合
}


//////////////////////////////////////////////
// 广播场景玩家组队信息
// type: MESSAGE_TEAM_ACTORS_BROADCAST
// dir: gateway->client
struct ActorTeamFields {
  1 : required string actor_;                           // 玩家ID
  2 : required string team_;                            // 队伍ID
}
struct MessageTeamActorsBroadcast {
  1 : required list<ActorTeamFields> actors_;           // 玩家列表
}

