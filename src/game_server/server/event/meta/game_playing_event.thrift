include "game_event.thrift"
include "playing.thrift"

namespace cpp game.server.event
namespace py ccevent.playing


////////////////////////////////////////////
// 副本创建
// type: EVENT_PLAYING_CREATE
struct EventPlayingCreate {
  1 : required i64 playing_;                // 副本ID
  2 : required i32 template_;               // 副本模板ID
  3 : required i64 scene_;                  // 场景ID
}


////////////////////////////////////////////
// 副本销毁
// type: EVENT_PLAYING_DESTORY
struct EventPlayingDestory {
  1 : required i64 playing_;                // 副本ID
  2 : required i32 template_;               // 副本模板ID
}


////////////////////////////////////////////
// 玩家进入副本
// type: EVENT_PLAYING_ACTOR_ENTER
struct EventPlayingActorEnter {
  1 : required i64 actor_;                  // 玩家ID
  2 : required i64 playing_;                // 副本ID
  3 : required i32 template_;               // 副本模板ID
}


////////////////////////////////////////////
// 玩家离开副本
// type: EVENT_PLAYING_ACTOR_LEAVE
struct EventPlayingActorLeave {
  1 : required i64 actor_;                  // 玩家ID
  2 : required i64 playing_;                // 副本ID
  3 : required i32 template_;               // 副本模板ID
}


////////////////////////////////////////////
// 玩家要求完成副本
// type: EVENT_PLAYING_ACTOR_REQUEST_FINISH
struct EventPlayingActorRequestFinish {
  1 : required i64 actor_;                  // 玩家ID
  2 : required i64 playing_;                // 副本ID
  3 : required i32 template_;               // 副本模板ID
}


////////////////////////////////////////////
// 玩家要求完成副本
// type: EVENT_PLAYING_ACTOR_REQUEST_COMPLETE
struct EventPlayingActorRequestComplete {
  1 : required i64 actor_;                  // 玩家ID
  2 : required i64 playing_;                // 副本ID
  3 : required i32 template_;               // 副本模板ID
}


////////////////////////////////////////////
// 请求更新记录
// type: EVENT_PLAYING_REQUEST_UPDATE_RECORD
struct EventPlayingRequestUpdateRecord {
  1 : required i64 actor_;                          // 玩家ID
  2 : required i32 template_;                       // 副本模板ID
  3 : required playing.PlayingResultField result_;  // 副本结果
  4 : required playing.PlayingRecordField record_;  // 副本当前记录
}


////////////////////////////////////////////
// 请求更新记录
// type: EVENT_PLAYING_REQUEST_AUTO_FINISH
struct EventPlayingRequestAutoFinish {
  1 : required i64 actor_;                          // 玩家ID
  2 : required i32 template_;                       // 副本模板ID
  3 : required playing.PlayingRecordField record_;  // 副本记录
  4 : required playing.PlayingResultField result_;  // 当前副本结果
  5 : required i32 finish_count_;                   // 完成次数
}


////////////////////////////////////////////
// 请求增加副本奖励
// type: EVENT_PLAYING_REQUEST_ADD_AWARD
struct EventPlayingRequestAddAward {
  1 : required i64 actor_;                  // 玩家ID
  2 : required i32 template_;               // 副本模板ID
  3 : required i32 score_;                  // 分数
}


////////////////////////////////////////////
// 副本完成事件
// type: EVENT_PLAYING_ACTOR_FINISH
struct EventPlayingActorFinish {
  1 : required i64 actor_;                  // 玩家ID
  2 : required i32 template_;               // 副本模板ID
}


////////////////////////////////////////////
// 玩家请求领取奖励
// type: EVENT_PLAYING_ACTOR_REQUEST_AWARD
struct EventPlayingActorRequestAward {
  1 : required i64 actor_;                            // 玩家ID
  2 : required i32 template_;                         // 副本模板ID
  3 : required playing.PlayingResultField result_;    // 副本结果
  4 : required playing.PlayingRecordField record_;    // 副本记录
}

