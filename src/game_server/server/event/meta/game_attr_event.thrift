
namespace cpp game.server.event
namespace py ccevent.attr


/////////////////////////////
// 精灵培养
// type: EVENT_ATTR_SPIRIT_FEED
struct EventAttrSpiritFeed {
  1 : required i32 actor_id_;         // 玩家id  
}

/////////////////////////////
// 星座升级
// type: EVENT_ATTR_CONSTELLATION_UP
struct EventAttrConstellationUpgrade {
  1 : required i64 actor_id_;         // 玩家id
}

/////////////////////////////
// 精灵进阶
// type: EVENT_ATTR_SPIRIT_UPGRADE
struct EventAttrSpiritUpgrade {
  1 : required i64 actor_id_;         // 玩家ID
  2 : required i32 old_level_;        // 原等级
  3 : required i32 new_level_;        // 新等级
}

/////////////////////////////
// 魔剑进阶
// type: EVENT_ATTR_SWORD_UPGRADE
struct EventAttrSwordUpgrade {
  1 : required i64 actor_id_;         // 玩家ID
  2 : required i32 old_step_;         // 原阶级
  3 : required i32 new_step_;         // 新阶级
}

/////////////////////////////
// 贵族(爵位)进阶
// type: EVENT_ATTR_NOBLE_UPGRADE
struct EventAttrNobleUpgrade {
  1 : required i64 actor_id_;         // 玩家ID
  2 : required i32 old_level_;        // 原等级
  3 : required i32 new_level_;        // 新等级
}

