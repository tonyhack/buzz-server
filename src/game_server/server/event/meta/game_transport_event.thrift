
namespace cpp game.server.event
namespace py ccevent.transport


///////////////////////////////
// 护送失败
// type: EVENT_TRANSPORT_FAILURE
struct EventTransportFailure {
  1 : required i64 actor_;                // 玩家ID
  2 : optional i64 rob_actor_;            // 抢夺玩家
}

