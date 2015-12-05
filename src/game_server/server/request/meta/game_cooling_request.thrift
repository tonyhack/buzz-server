include "game_request.thrift"
include "entity.thrift"

namespace cpp game.server.request
namespace py ccrequest.cooling


///////////////////////////////////////
// 请求检测是否冷却中
// type: REQUEST_CHECK_IN_COOLING
struct RequestCheckInCooling {
  1 : required entity.EntityType type_;           // 类型         (IN)
  2 : required i64 id_;                           // ID           (IN)
  3 : required i32 cooling_id_;                   // 冷却ID       (IN)
  4 : required bool in_cooling_;                  // 结果         (OUT)
}


///////////////////////////////////////
// 请求开启冷却
// type: REQUEST_START_COOLING
struct RequestStartCooling {
  1 : required entity.EntityType type_;           // 类型         (IN)
  2 : required i64 id_;                           // ID           (IN)
  3 : required i32 cooling_id_;                   // 冷却ID       (IN)
  4 : required i64 cooling_add_value_ = 0;        // 冷却附加值   (IN)
  5 : required i32 cooling_add_percent_ = 0;      // 冷却附加比   (IN)
}

