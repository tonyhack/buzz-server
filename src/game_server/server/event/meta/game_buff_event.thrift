include "entity.thrift"
include "game_event.thrift"

namespace cpp game.server.event
namespace py ccevent.buff


/////////////////////////////////
// BUFF开启
// type: EVENT_BUFF_START
struct EventBuffStart {
  1 : required entity.EntityType type_;         // 类型
  2 : required i64 id_;                         // ID
  3 : required i32 buff_;                       // BUFF
  4 : required i32 stack_count_;                // 叠加数量
}


/////////////////////////////////
// BUFF关闭
// type: EVENT_BUFF_END
struct EventBuffEnd {
  1 : required entity.EntityType type_;         // 类型
  2 : required i64 id_;                         // ID
  3 : required i32 buff_;                       // BUFF
}

