include "session_request.thrift"

namespace cpp session.server.request


////////////////////////////////////
// 获取玩家关联的副本信息
// type: REQUEST_PLAYING_ACTOR_CONNECT_INFO
struct RequestPlayingActorConnectInfo {
  1 : required i64 req_actor_;                          // 玩家ID
  2 : required i32 ret_playing_;                        // 副本ID
  3 : required i32 ret_playing_template_;               // 副本模板ID
  4 : required i64 ret_scene_;                          // 副本场景ID
  5 : required bool ret_online_back_to_;                // 上线后是否需要自动回到副本
}

