include "entity.thrift"
include "game_event.thrift"

namespace cpp game.server.event
namespace py ccevent.ai

//////////////////////////////////////////
// 仇恨开始(进入仇恨列表)
// type: EVENT_AI_THREAT_START
struct EventAiThreatStart {
  1 : required entity.EntityType threat_role_type_;         // 被仇恨的角色类型
  2 : required i64 threat_role_id_;                         // 被仇恨的角色ID
  3 : required i64 npc_;                                    // NPC的ID
}


//////////////////////////////////////////
// 仇恨结束(从仇恨列表清除)
// type: EVENT_AI_THREAT_END
struct EventAiThreatEnd {
  1 : required entity.EntityType threat_role_type_;         // 被仇恨的角色类型
  2 : required i64 threat_role_id_;                         // 被仇恨的角色ID
  3 : required i64 npc_;                                    // NPC的ID
}

