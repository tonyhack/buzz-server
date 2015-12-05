include "playing.thrift"
include "session_game_protocol.thrift"

namespace cpp session.protocol


/////////////////////////////////////////
// 创建副本请求
// type: MESSAGE_PLAYING_CREATE_REQUEST
// dir: game->session
struct MessagePlayingCreateRequest {
  1 : required i64 actor_;                          // 玩家ID
  2 : required i32 template_;                       // 副本模板ID
}


/////////////////////////////////////////
// 创建副本回应
// type: MESSAGE_PLAYING_CREATE_RESPONSE
// dir: session->game
struct MessagePlayingCreateResponse {
  1 : required i64 actor_;                          // 玩家ID
  2 : required i32 template_;                       // 副本模板ID
  3 : required i64 playing_;                        // 副本ID
  4 : required bool result_;                        // 结果
}


/////////////////////////////////////////
// 创建副本同步
// type: MESSAGE_PLAYING_CREATE_SYNCHRONIZE
// dir: session->game
struct MessagePlayingCreateSynchronize {
  1 : required i64 playing_;                        // 副本ID
  2 : required i32 template_;                       // 副本模板ID
}


/////////////////////////////////////////
// 创建副本完成请求
// type: MESSAGE_PLAYING_CREATE_FINISH_REQUEST
// dir: game->session
struct MessagePlayingCreateFinishRequest {
  1 : required i64 playing_;                        // 副本ID
  2 : required i64 scene_;                          // 场景ID
  3 : optional byte failure_;                       // 失败标志
}


/////////////////////////////////////////
// 创建副本完成回应
// type: MESSAGE_PLAYING_CREATE_FINISH_RESPONSE
// dir: session->game
struct MessagePlayingCreateFinishResponse {
  1 : required i64 playing_;                        // 副本ID
  2 : required bool result_;                        // 结果
}


/////////////////////////////////////////
// 玩家进入副本同步
// type: MESSAGE_PLAYING_ENTER_SYNCHRONIZE
// dir: session->game
struct MessagePlayingEnterSynchronize {
  1 : required i64 actor_;                          // 玩家ID
  2 : required i64 playing_;                        // 副本ID
  3 : required i64 scene_;                          // 副本场景ID
}


/////////////////////////////////////////
// 销毁副本请求
// type: MESSAGE_PLAYING_DESTORY_REQUEST
// dir: game->session
struct MessagePlayingDestoryRequest {
  1 : required i64 playing_;                        // 副本ID
}


/////////////////////////////////////////
// 销毁副本回应
// type: MESSAGE_PLAYING_DESTORY_RESPONSE
// dir: session->game
struct MessagePlayingDestoryResponse {
  1 : required i64 playing_;                        // 副本ID
  2 : required bool result_;                        // 结果
}


//////////////////////////////////////////
// 副本结束请求
// type: MESSAGE_PLAYING_END_REQUEST
// dir: game->session
struct MessagePlayingEndRequest {
  1 : required i64 playing_;                        // 副本ID
}


//////////////////////////////////////////
// 副本结束回应
// type: MESSAGE_PLAYING_END_RESPONSE
// dir: session->game
struct MessagePlayingEndResponse {
  1 : required i64 playing_;                        // 副本ID
  2 : required bool result_;                        // 结果
}


//////////////////////////////////////////
// 玩家副本数据同步
// type: MESSAGE_PLAYING_ACTOR_PAGE_SYNCHRONIZE
// dir: game->session
struct PlayingPageField {
  1 : required i32 template_;                       // 副本模板ID
  2 : required playing.PlayingRecordField record_;  // 副本记录
  3 : required playing.PlayingResultField result_;  // 副本结果
}
struct MessagePlayingActorPageSynchronize {
  1 : required i64 actor_;                          // 玩家ID
  2 : required list<PlayingPageField> pages_;       // 副本数据
}


//////////////////////////////////////////
// 副本队伍同步
// type: MESSAGE_PLAYING_TEAM_SYNCHRONIZE
// dir: session->game
struct MessagePlayingTeamSynchronize {
  1 : required i64 actor_;                          // 玩家ID
  2 : required i64 playing_team_;                   // 玩家所在副本队伍
}


//////////////////////////////////////////
// 地宫地图数据
struct MazeMapData {
  1 : required list<i32> grids_;                    // 地宫格子
}


//////////////////////////////////////////
// 地宫地图数据请求
// type: MESSAGE_MAZE_MAP_INFO_REQUEST
// dir: game->session
struct MessageMazeMapInfoRequest {
}


//////////////////////////////////////////
// 地宫地图数据同步
// type: MESSAGE_MAZE_MAP_INFO_SYNCHRONIZE
// dir: session->game
struct MessageMazeMapInfoSynchronize {
  1 : required list<MazeMapData> maps_;            // 地宫地图数据
  2 : required i64 last_update_time_;              // 上次更新时间
}

