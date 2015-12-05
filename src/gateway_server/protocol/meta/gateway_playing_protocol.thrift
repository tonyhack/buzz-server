include "gateway_protocol.thrift"
include "playing.thrift"
include "maze.thrift"

namespace cpp gateway.protocol
namespace as3 com.dreamfire.sanguo.data.definition.protocol.gateway


//////////////////////////////////////////
// 副本数据初始化
struct PlayingPageField {
  1 : required i32 template_;                                     // 副本模板ID
  2 : required playing.PlayingRecordField record_;                // 副本记录
  3 : required playing.PlayingResultField result_;                // 副本结果
}
struct MessagePlayingInitialize {
  1 : required list<PlayingPageField> playings_;                  // 副本数据
  2 : optional i32 auto_playing_;                                 // 自动进行中的副本(二选一)
  3 : optional i32 auto_playing_group_;                           // 自动进行中的副本组(二选一)
  4 : optional i32 auto_remain_secs_;                             // 自动进行的剩余秒数 
}


//////////////////////////////////////////
// 创建副本请求
// type: MESSAGE_PLAYING_CREATE_REQUEST
// dir: client->gateway
struct MessagePlayingCreateRequest {
  1 : required i32 template_;                                     // 副本模板ID
}


//////////////////////////////////////////
// 创建副本回应
// type: MESSAGE_PLAYING_CREATE_RESPONSE
// dir: gateway->client
struct MessagePlayingCreateResponse {
  1 : required i32 template_;                                     // 副本模板ID
  2 : required bool result_;                                      // 结果
}


//////////////////////////////////////////
// 创建副本完成通知
// type: MESSAGE_PLAYING_CREATE_FINISH_NOTICE
// dir: gateway->client
struct MessagePlayingCreateFinishNotice {
  1 : required i32 template_;                                     // 副本模板ID
  2 : required string playing_;                                   // 副本ID
}


//////////////////////////////////////////
// 进入副本请求
// type: MESSAGE_PLAYING_ENTER_REQUEST
// dir: client->gateway
struct MessagePlayingEnterRequest {
  1 : required string playing_;                                   // 副本ID
  2 : required i32 template_;                                     // 副本模板ID
}


//////////////////////////////////////////
// 进入副本回应
// type: MESSAGE_PLAYING_ENTER_RESPONSE
// dir: gateway->client
struct MessagePlayingEnterResponse {
  1 : required bool result_;                                      // 是否成功
  2 : required i32 template_;                                     // 副本模板ID
}


//////////////////////////////////////////
// 玩家进入副本初始化
// type: MESSAGE_PLAYING_ENTER_INITIALIZE
// dir: gateway->client
struct MessagePlayingEnterInitialize {
  1 : required i32 template_;                                     // 副本模块ID
  2 : required i32 spend_time_;                                   // 副本总花费时间
  3 : required list<playing.PlayingScoreField> scores_;           // 副本已得分的记录
  4 : required list<i32> datas_;                                  // 副本数据(根据具体副本进行解释)
}


//////////////////////////////////////////
// 副本得分同步
// type: MESSAGE_PLAYING_SCORE_SYNCHRONIZE
// dir: gateway->client
struct MessagePlayingScoreSynchronize {
  1 : required playing.PlayingScoreField score_;                  // 得分
}


//////////////////////////////////////////
// 副本得分同步
// type: MESSAGE_PLAYING_RANKING_SYNCHRONIZE
// dir: gateway->client
struct MessagePlayingRandkingSynchronize {
  1 : required list<playing.PlayingRankingField> ranking_;        // 排行
}


//////////////////////////////////////////
// 完成副本请求
// type: MESSAGE_PLAYING_COMPLETE_REQUEST
// dir: client->server
struct MessagePlayingCompleteRequest {
}


//////////////////////////////////////////
// 完成副本回应
// type: MESSAGE_PLAYING_COMPLETE_RESPONSE
// dir: server->client
struct MessagePlayingCompleteResponse {
  1 : required bool result_;                                      // 结果
}


//////////////////////////////////////////
// 完成副本同步
// type: MESSAGE_PLAYING_COMPLETE_SYNCHRONIZE
// dir: server->client
struct MessagePlayingCompleteSynchronize {
  1 : required playing.PlayingResultField result_;                // 副本结果
  2 : optional list<playing.PlayingAwardField> awards_;           // 奖励
  3 : optional playing.PlayingAwardField draw_award_;             // 抽奖奖励
}


//////////////////////////////////////////
// 离开副本请求
// type: MESSAGE_PLAYING_LEAVE_REQUEST
// dir: client->server
struct MessagePlayingLeaveRequest {
}


//////////////////////////////////////////
// 离开副本回应
// type: MESSAGE_PLAYING_LEAVE_RESPONSE
// dir: server->client
struct MessagePlayingLeaveResponse {
  1 : required bool result_;                                      // 结果
}


//////////////////////////////////////////
// 离开副本同步
// type: MESSAGE_PLAYING_LEAVE_SYNCHRONIZE
// dir: server->client
struct MessagePlayingLeaveSynchronize {
}


//////////////////////////////////////////
// 领取奖励请求
// type: MESSAGE_PLAYING_AWARD_REQUEST
// dir: client->server
struct MessagePlayingAwardRequest {
  1 : optional i32 playing_template_;                             // 副本模板ID
  2 : optional i32 playing_group_;                                // 副本组
}


//////////////////////////////////////////
// 领取奖励回应
// type: MESSAGE_PLAYING_AWARD_RESPONSE
// dir: server->client
struct MessagePlayingAwardResponse {
  1 : required bool result_;                                      // 结果
}


//////////////////////////////////////////
// 领取奖励回应
// type: MESSAGE_PLAYING_RECORD_SYNCHRONIZE
// dir: server->client
struct MessagePlayingRecordSynchronize {
  1 : required i32 playing_template_;                             // 副本模板ID
  2 : required playing.PlayingRecordField record_;                // 副本记录
  3 : required playing.PlayingResultField result_;                // 副本上次结果
}


//////////////////////////////////////////
// 副本自动进行请求
// type: MESSAGE_PLAYING_AUTO_REQUEST
// dir: client->server
struct MessagePlayingAutoRequest {
  1 : optional i32 playing_template_;                             // 副本ID
  2 : optional i32 playing_group_;                                // 副本组ID
}


//////////////////////////////////////////
// 副本自动进行回应
// type: MESSAGE_PLAYING_AUTO_RESPONSE
// dir: gateway->client
struct MessagePlayingAutoResponse {
  1 : required bool result_;                                      // 结果
  2 : optional i32 playing_template_;                             // 副本ID
  3 : optional i32 playing_group_;                                // 副本组ID
}


//////////////////////////////////////////
// 同步结束自动进行副本
// type: MESSAGE_PLAYING_AUTO_STOP_SYNCHRONIZE
// dir: gateway->client
struct MessagePlayingAutoStopSynchronize {
  1 : required bool finish_;                                      // 是否正常完成
  2 : optional i32 playing_template_;                             // 副本ID
  3 : optional i32 playing_group_;                                // 副本组ID
}


//////////////////////////////////////////
// 副本自动进行完成请求
// type: MESSAGE_PLAYING_AUTO_FINISH_REQUEST
// dir: client->gateway
struct MessagePlayingAutoFinishRequest {
  1 : optional i32 playing_template_;                             // 副本ID
  2 : optional i32 playing_group_;                                // 副本组ID
}


//////////////////////////////////////////
// 副本自动进行完成回应
// type: MESSAGE_PLAYING_AUTO_FINISH_RESPONSE
// dir: gateway->client
struct MessagePlayingAutoFinishResponse {
  1 : required bool result_;                                      // 是否成功
  2 : optional i32 playing_template_;                             // 副本ID
  3 : optional i32 playing_group_;                                // 副本组ID
}


//////////////////////////////////////////
// 副本有偿奖励请求
// type: MESSAGE_PLAYING_PAID_AWARD_REQUEST
// dir: cilent->gateway
struct MessagePlayingPaidAwardRequest {
  1 : required i32 playing_template_;                             // 副本ID
}


//////////////////////////////////////////
// 副本队伍列表请求
// type: MESSAGE_PLAYING_TEAM_LIST_REQUEST
// dir: client->gateway
struct MessagePlayingTeamListRequest {
  1 : required i32 playing_template_;                             // 副本ID
}


//////////////////////////////////////////
// 副本队伍列表回应
// type: MESSAGE_PLAYING_TEAM_LIST_RESPONSE
// dir: gateway->client
struct MessagePlayingTeamListResponse {
  1 : required i32 playing_template_;                             // 副本ID
  2 : required list<playing.PlayingTeamField> teams_;             // 队伍列表
}


//////////////////////////////////////////
// 创建副本队伍请求
// type: MESSAGE_PLAYING_TEAM_CREATE_REQUEST
// dir: client->gateway
struct MessagePlayingTeamCreateRequest {
  1 : required i32 playing_template_;                             // 副本ID
  2 : required i32 min_fight_score_;                              // 最小战斗力
  3 : required bool auto_open_;                                   // 自动开启
}


//////////////////////////////////////////
// 创建副本队伍回应
// type: MESSAGE_PLAYING_TEAM_CREATE_RESPONSE
// dir: gateway->client
struct MessagePlayingTeamCreateResponse {
  1 : required bool result_;                                      // 是否成功
  2 : required i32 playing_template_;                             // 副本ID
}


//////////////////////////////////////////
// 组队信息同步
// type: MESSAGE_PLAYING_TEAM_SYNCHRONIZE
// dir: gateway->client
struct MessagePlayingTeamSynchronize {
  1 : required string id_;                                        // 队伍ID
  2 : optional string name_;                                      // 队伍名称
  3 : optional playing.PlayingTeamStatus status_;                 // 队伍状态
  4 : optional i32 capacity_;                                     // 队伍容量
  5 : optional i32 min_fight_score_;                              // 最小战斗力
  6 : optional bool auto_open_;                                   // 自动开启
  7 : optional list<playing.PlayingTeamMemberField> members_;     // 队员信息
}


//////////////////////////////////////////
// 队员信息同步
// type: MESSAGE_PLAYING_TEAM_MEMBER_SYNCHRONIZE
// dir: gateway->client
struct MessagePlayingTeamMemberSynchronize {
  1 : required string actor_;                                     // 玩家ID
  2 : optional string name_;                                      // 名称
  3 : optional i32 fight_score_;                                  // 战斗力
  4 : optional i32 level_;                                        // 当前等级
  5 : optional bool ready_;                                       // 是否已准备
}


//////////////////////////////////////////
// 加入副本队伍请求
// type: MESSAGE_PLAYING_TEAM_JOIN_REQUEST
// dir: client->gateway
struct MessagePlayingTeamJoinRequest {
  1 : required string team_id_;                                   // 队伍ID
}


//////////////////////////////////////////
// 加入副本队伍回应
// type: MESSAGE_PLAYING_TEAM_JOIN_RESPONSE
// dir: gateway->client
struct MessagePlayingTeamJoinResponse {
  1 : required bool result_;                                      // 是否成功
}


//////////////////////////////////////////
// 加入副本队伍同步
// type: MESSAGE_PLAYING_TEAM_JOIN_SYNCHRONIZE
// dir: gateway->client
struct MessagePlayingTeamJoinSynchronize {
  1 : required string actor_;                                     // 玩家ID
  2 : required string name_;                                      // 名称
  3 : required i32 fight_score_;                                  // 战斗力
  4 : required i32 level_;                                        // 当前等级
  5 : required bool ready_;                                       // 是否已准备
}


//////////////////////////////////////////
// 退出副本队伍请求
// type: MESSAGE_PLAYING_TEAM_QUIT_REQUEST
// dir: client->gateway
struct MessagePlayingTeamQuitRequest {
}


//////////////////////////////////////////
// 退出副本队伍回应
// type: MESSAGE_PLAYING_TEAM_QUIT_RESPONSE
// dir: gateway->client
struct MessagePlayingTeamQuitResponse {
  1 : required bool result_;                                      // 是否成功
}


//////////////////////////////////////////
// 退出副本队伍同步
// type: MESSAGE_PLAYING_TEAM_QUIT_SYNCHRONIZE
// dir: gateway->client
struct MessagePlayingTeamQuitSynchronize {
  1 : required string actor_;                                     // 玩家ID
}


//////////////////////////////////////////
// 准备副本请求
// type: MESSAGE_PLAYING_TEAM_READY_REQUEST
// dir: client->gateway
struct MessagePlayingTeamReadyRequest {
  1 : required bool ready_;                                       // 准备或取消
}


//////////////////////////////////////////
// 准备副本回应
// type: MESSAGE_PLAYING_TEAM_READY_RESPONSE
// dir: gateway->client
struct MessagePlayingTeamReadyResponse {
  1 : required bool result_;                                      // 是否成功
}


//////////////////////////////////////////
// 踢出副本队伍请求
// type: MESSAGE_PLAYING_TEAM_KICKOUT_REQUEST
// dir: client->gateway
struct MessagePlayingTeamKickoutRequest {
  1 : required string actor_;                                     // 玩家ID
}


//////////////////////////////////////////
// 踢出副本队伍回应
// type: MESSAGE_PLAYING_TEAM_KICKOUT_RESPONSE
// dir: gateway->client
struct MessagePlayingTeamKickoutResponse {
  1 : required bool result_;                                      // 是否成功
}


//////////////////////////////////////////
// 设置副本队伍空位请求
// type: MESSAGE_PLAYING_TEAM_CAPACITY_SET_REQUEST
// dir: client->gateway
struct MessagePlayingTeamCapacitySetRequest {
  1 : required i32 capacity_;                                     // 空位个数
}


//////////////////////////////////////////
// 设置副本队伍空位回应
// type: MESSAGE_PLAYING_TEAM_CAPACITY_SET_RESPONSE
// dir: gateway->client
struct MessagePlayingTeamCapacitySetResponse {
  1 : required bool result_;                                      // 是否成功
}


//////////////////////////////////////////
// 副本有偿奖励回应
// type: MESSAGE_PLAYING_PAID_AWARD_RESPONSE
// dir: gateway->client
struct MessagePlayingPaidAwardResponse {
  1 : required bool result_;                                      // 是否成功
  2 : required i32 playing_template_;                             // 副本ID
}


//////////////////////////////////////////
// 地宫数据初始化
// type: MESSAGE_MAZE_INITIALIZE
// dir: gateway->client
struct MessageMazeInitialize {
  1 : required i32 action_point_;                                 // 当前行动点
  2 : required i32 action_point_recover_cooling_;                 // 行动点恢复剩余时间
  3 : required i32 floor_;                                        // 玩家所在地宫层数
  4 : required i32 x_;                                            // 玩家坐标x
  5 : required i32 y_;                                            // 玩家坐标y
  6 : required list<i32> grids_;                                  // 地宫格子
  7 : required i32 gift_num_;                                     // 剩余未领取地宫通关礼包数量
}


//////////////////////////////////////////
// 地宫数据同步
// type: MESSAGE_MAZE_SYNCHRONIZE
// dir: gateway->client
struct MessageMazeSynchronize {
  1 : optional i32 action_point_;                                 // 当前行动点
  2 : optional i32 action_point_recover_cooling_;                 // 行动点恢复剩余时间
  3 : optional i32 x_;                                            // 玩家坐标x
  4 : optional i32 y_;                                            // 玩家坐标y
  5 : optional i32 gift_num_;                                     // 剩余未领取地宫通关礼包数量
}


//////////////////////////////////////////
// 地宫进入下一层通知 
// type: MESSAGE_MAZE_ENTER_NEXT_FLOOR_NOTIFY
// dir: gateway->client
struct MessageMazeEnterNextFloorNotify {
  1 : required i32 floor_;                                        // 玩家所在地宫层数
  2 : required list<i32> grids_;                                  // 地宫格子
}


//////////////////////////////////////////
// 地宫移动请求
// type: MESSAGE_MAZE_MOVE_REQUEST
// dir: client->gateway
struct MessageMazeMoveRequest {
  1 : required maze.MazeDirectionType dir_;                       // 移动方向
}


//////////////////////////////////////////
// 地宫移动回复
// type: MESSAGE_MAZE_MOVE_RESPONSE
// dir: gateway->client
struct MessageMazeMoveResponse {
  1 : required bool result_;                                      // 结果
  2 : optional i32 grid_id_;                                      // 格子ID
}


//////////////////////////////////////////
// 地宫确认移动请求
// type: MESSAGE_MAZE_CONFIRM_MOVE_REQUEST
// dir: client->gateway
struct MessageMazeConfirmMoveRequest {
  1 : required maze.MazeDirectionType dir_;                       // 移动方向
}


//////////////////////////////////////////
// 地宫确认移动回复
// type: MESSAGE_MAZE_CONFIRM_MOVE_RESPONSE
// dir: gateway->client
struct MessageMazeConfirmMoveResponse {
  1 : required bool result_;                                      // 结果
  2 : optional i32 grid_id_;                                      // 格子ID
  3 : optional i32 bonus_id_;                                     // 奖励ID
  4 : optional i32 number_;                                       // 数量
}


//////////////////////////////////////////
// 地宫领取奖励请求
// type: MESSAGE_MAZE_AWARD_REQUEST
// dir: client->gateway
struct MessageMazeAwardRequest {
}


//////////////////////////////////////////
// 地宫领取奖励回复
// type: MESSAGE_MAZE_AWARD_RESPONSE
// dir: gateway->client
struct MessageMazeAwardResponse {
  1 : required bool result_;                                      // 结果
}

