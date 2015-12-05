include "game_event.thrift"

namespace cpp game.server.event
namespace py ccevent.soul


//////////////////////////////////////////
// 增加英灵
// type: EVENT_SOUL_ADD
struct EventSoulAdd {
  1 : required i64 actor_;                // 玩家ID
  2 : required i32 soul_;                 // 英灵ID
}

//////////////////////////////////////////
// 英灵设置
// type: EVENT_SOUL_SET
struct EventSoulSet {
  1 : required i64 actor_;                // 玩家ID
  2 : required i32 soul_;                 // 英灵ID
}

//////////////////////////////////////////
// 英灵升阶
// type: EVENT_SOUL_UPGRADE_STEP
struct EventSoulUpgradeStep {
  1 : required i64 actor_;                // 玩家ID
  2 : required i32 soul_;                 // 英灵ID
  3 : required i32 step_;                 // 英灵当前阶级
}

//////////////////////////////////////////
// 英灵升级
// type: EVENT_SOUL_UPGRADE_LEVEL
struct EventSoulUpgradeLevel {
  1 : required i64 actor_;                // 玩家ID
  2 : required i32 soul_;                 // 英灵ID
  3 : required i32 level_;                // 英灵当前等级
}

