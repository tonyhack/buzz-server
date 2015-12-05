include "entity.thrift"
include "game_event.thrift"

namespace cpp game.server.event
namespace py ccevent.social
///////////////////////////////////////////

// 掠夺
// type: EVENT_SOCIAL_HOMELAND_BATTLE_STRIKE
struct EventSocialHomeLandBattleStrike {
  1 : required i64 actor_;                // 掠夺
}

// 浇水
// type: EVENT_SOCIAL_HOMELAND_TREE_WATER
struct EventSocialHomeLandTreeWater {
  1 : required i64 id_;                   // 执行者
  2 : required i64 watered_id_;           // 被执行者
}


