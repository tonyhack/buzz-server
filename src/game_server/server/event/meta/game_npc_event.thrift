include "game_event.thrift"

namespace cpp game.server.event
namespace py ccevent.npc


// NPC创建事件
// type: EVENT_NPC_CREATE
struct EventNpcCreate {
  1 : required i64 id_;
  2 : required bool reborn_;
  3 : optional i32 reborn_secs_;
}


// NPC销毁事件
// type: EVENT_NPC_DESTROY
struct EventNpcDestory {
  1 : required i64 id_;
}

