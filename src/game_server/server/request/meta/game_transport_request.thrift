include "game_request.thrift"

namespace cpp game.server.request
namespace py ccrequest.transport


////////////////////////////////////
// 请求召唤护送
// type: REQUEST_TRANSPORT_SUMMON
struct RequestTransportSummon {
  1 : required i64 actor_;                        // 玩家ID           (IN)
  2 : required i32 transport_npc_template_;       // 护送NPC的模板ID  (IN)
  3 : required i64 transport_npc_;                // 护送NPC的ID      (OUT)
}

////////////////////////////////////
// 请求清除护送
// type: REQUEST_TRANSPORT_REMOVE
struct RequestTransportRemove {
  1 : optional i64 actor_;                        // 玩家ID           (IN)
  2 : optional i64 transport_npc_;                // 护送NPC的ID      (IN)
}

////////////////////////////////////
// 请求护送NPC信息
// type: REQUEST_TRANSPORT_CURRENT_NPC
struct RequestTransportCurrentNpc {
  1 : required i64 actor_;                        // 玩家ID           (IN)
  2 : required i64 transport_npc_;                // 护送NPC的ID      (OUT)
  3 : required i32 transport_npc_template_;       // 护送NPC的模板ID  (OUT)
}

////////////////////////////////////
// 请求检测护送NPC是否在周围
// type: REQUEST_TRANSPORT_AROUND
struct RequestTransportAround {
  1 : required i64 actor_;                        // 玩家ID           (IN)
  2 : required bool around_;                      // 是否在周围       (OUT)
}

////////////////////////////////////
// 请求获取护送快照
// type: REQUEST_TRANSPORT_GET_SNAPSHOT
struct RequestTransportGetSnapshot {
  1 : required i64 transport_npc_id_;             // 护送NPC的ID      (IN)
  2 : required binary snapshot_;                  // 快照             (OUT)
}

////////////////////////////////////
// 请求根据快照召唤护送
// type: REQUEST_TRANSPORT_SUMMON_FROM_SNAPSHOT
struct RequestTransportSummonFromSnapshot {
  1 : required i32 transport_npc_template_;       // 护送NPC的模板ID  (IN)
  2 : required i64 scene_;                        // 场景ID           (IN)
  3 : required i32 x_;                            // 坐标x            (IN)
  4 : required i32 y_;                            // 坐标y            (IN)
  5 : required binary snapshot_;                  // 快照             (IN)
  6 : required i64 transport_npc_;                // 护送NPC的ID      (OUT)
}

