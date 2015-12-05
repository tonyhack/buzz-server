include "game_request.thrift"
include "playing.thrift"

namespace cpp game.server.request
namespace py ccrequest.playing


//////////////////////////////////////
// 设置副本玩家变量
// type: REQUEST_PLAYING_SET_ACTOR_VARIABLES
struct RequestPlayingSetActorVariables {
  1 : required i64 actor_;                                // 玩家ID
  2 : required list<string> keys_;                        // 变量名
  3 : required list<i64> values_;                         // 变量值
  4 : required bool result_;                              // 结果
}


//////////////////////////////////////
// 获取副本玩家变量
// type: REQEUST_PLAYING_GET_ACTOR_VARIABLES
struct RequestPlayingGetActorVariables {
  1 : required i64 actor_;                                // 玩家ID
  2 : required list<string> keys_;                        // 变量名
  3 : required list<i64> values_;                         // 变量值
  4 : required bool result_;                              // 结果
}


//////////////////////////////////////
// 副本玩家变量增长
// type: REQUEST_PLAYING_INCREASE_ACTOR_VARIABLES
struct RequestPlayingIncreaseActorVariables {
  1 : required i64 actor_;                                // 玩家ID
  2 : required list<string> keys_;                        // 变量名
  3 : required list<i64> increments_;                     // 增长值
  4 : required list<i64> values_;                         // 变量值
  5 : required bool result_;                              // 结果
}


//////////////////////////////////////
// 副本玩家变量减小
// type: REQUEST_PLAYING_DECREASE_ACTOR_VARIABLES
struct RequestPlayingDecreaseActorVariables {
  1 : required i64 actor_;                                // 玩家ID
  2 : required list<string> keys_;                        // 变量名
  3 : required list<i64> decrements_;                     // 减小值
  4 : required list<i64> values_;                         // 变量值
  5 : required bool result_;                              // 结果
}


//////////////////////////////////////
// 请求初始化玩家数据
// REQUEST_PLAYING_INITIALIZE_ACTOR
struct RequestPlayingInitializeActor {
  1 : required i64 actor_;                                // 玩家ID
  2 : required i32 spend_time_;                           // 已经花费的时间(秒)
  3 : required list<playing.PlayingScoreField> scores_;   // 分数信息
  4 : required list<i32> datas_;                          // 副本数据(根据具体副本进行解释)
}


//////////////////////////////////////
// 请求副本得分
// type: REQUEST_PLAYING_SYNCHRONIZE_SCORE
struct RequestPlayingSynchronizeScore {
  1 : required i64 actor_;                                // 玩家ID
  2 : required playing.PlayingScoreField score_;          // 得分
}


//////////////////////////////////////
// 请求同步排行榜
// type: REQUEST_PLAYING_SYNCHRONIZE_RANKING
struct RequestPlayingSynchronizeRanking {
  1 : required list<playing.PlayingRankingField> ranking_;  // 排行
  2 : required list<i64> actors_;                           // 玩家列表
}


//////////////////////////////////////
// 请求副本完成
// type: REQUEST_PLAYING_COMPLETE
struct RequestPlayingComplete {
  1 : required i64 playing_;                              // 副本ID
  2 : required playing.PlayingResultField result_;        // 副本结果
  3 : optional list<playing.PlayingAwardField> awards_;   // 奖励
  4 : optional playing.PlayingAwardField draw_award_;     // 抽奖奖励
}


//////////////////////////////////////
// 请求某玩家副本完成
// type: REQUEST_PLAYING_ACTOR_COMPLETE
struct RequestPlayingActorComplete {
  1 : required i64 playing_;                              // 副本ID
  2 : required i64 actor_;                                // 玩家ID
  3 : required playing.PlayingResultField result_;        // 副本结果
  4 : optional list<playing.PlayingAwardField> awards_;   // 奖励
  5 : optional playing.PlayingAwardField draw_award_;     // 抽奖奖励
}


//////////////////////////////////////
// 请求副本失败
// type: REQUEST_PLAYING_FAILURE
struct RequestPlayingFailure {
  1 : required i64 playing_;                              // 副本ID
}


//////////////////////////////////////
// 请求更新副本记录
// type: REQUEST_PLAYING_UPDATE_RECORD
struct RequestPlayingUpdateRecord {
  1 : required i64 actor_;                                // 玩家ID
  2 : required i32 playing_template_;                     // 副本ID
  3 : required playing.PlayingRecordField record_;        // 新记录
}


//////////////////////////////////////
// 请求更新副本结果
// type: REQUEST_PLAYING_UPDATE_RESULT
struct RequestPlayingUpdateResult {
  1 : required i64 actor_;                                // 玩家ID
  2 : required i32 playing_template_;                     // 副本ID
  3 : required playing.PlayingResultField result_;        // 副本结果
}


//////////////////////////////////////
// 请求增加副本奖励
// type: REQUEST_PLAYING_ADD_AWARD
struct RequestPlayingAddAward {
  1 : required i64 actor_;                                // 玩家ID
  2 : required i32 playing_template_;                     // 副本ID
  3 : required list<playing.PlayingAwardField> awards_;   // 奖励
  4 : required playing.PlayingAwardField draw_award_;     // 抽奖奖励
}


//////////////////////////////////////
// 请求清空副本完成次数
// type: REQUEST_PLAYING_CLEAR_COMPLETE
struct RequestPlayingClearComplete {
  1 : required i64 actor_;                                // 玩家ID
  2 : required i32 playing_template_;                     // 副本ID
}


//////////////////////////////////////
// 请求增加副本完成次数
// type: REQUEST_PLAYING_INCREASE_COMPLETE
struct RequestPlayingIncreaseComplete {
  1 : required i64 playing_;                              // 副本
}


//////////////////////////////////////
// 请求增加单个玩家副本完成次数
// type: REQUEST_PLAYING_ACTOR_INCREASE_COMPLETE
struct RequestPlayingActorIncreaseComplete {
  1 : required i64 actor_;                                // 玩家ID
  2 : required i32 playing_template_;                     // 副本ID
}


//////////////////////////////////////
// 请求奖励副本玩家
// type: REQUEST_PLAYING_AWARD_ACTOR
struct RequestPlayingAwardActor {
  1 : required i64 actor_;                                // 玩家ID
  2 : required i32 playing_template_;                     // 副本ID
  3 : required bool first_pass_award_ = false;            // 是否为首通奖励
  4 : required list<playing.PlayingAwardField> awards_;   // 奖励
}


/////////////////////////////////////
// 获取玩家所在的副本
// type: REQUEST_PLAYING_ACTOR_PLAYING_ID
struct RequestPlayingActorPlayingID {
  1 : required i64 actor_;                                // 玩家
  2 : required i32 playing_id_;                           // 副本ID
}
