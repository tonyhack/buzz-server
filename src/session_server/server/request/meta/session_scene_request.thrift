include "session_request.thrift"

namespace cpp session.server.request

////////////////////////////////////////////
// 请求获取静态地图的场景ID
// type: REQUEST_GET_MAP_SCENE_ID
struct RequestGetMapSceneID {
  1 : required i16 map_;                              // 静态地图ID
  2 : required i64 scene_;                            // 场景ID
}


////////////////////////////////////////////
// 请求获取某地图上的玩家集合
// type: REQUEST_SCENE_GET_ACTORS
struct RequestSceneGetActors {
  1 : required i64 scene_;                            // 场景ID
  2 : required list<i64> actors_;                     // 玩家集合
}


////////////////////////////////////////////
// 请求获取场景所在GameServer
// type: REQUEST_SCENE_GET_GAME_SERVER
struct RequestSceneGetGameServer {
  1 : optional i32 map_;                              // 静态地图ID(命中时，填充scene_)
  2 : optional i64 scene_;                            // 场景ID
  3 : required i32 game_server_;                      // GameServer
}


////////////////////////////////////////////
// 请求跳场景地图
// type: REQUEST_SCENE_JUMP
struct RequestSceneJump {
  1 : required i64 actor_;                            // 玩家ID
  2 : optional i32 map_;                              // 静态地图ID(命中时，填充scene_)
  3 : optional i64 scene_;                            // 场景ID
  4 : required i32 x_;                                // 坐标x
  5 : required i32 y_;                                // 坐标y
}

