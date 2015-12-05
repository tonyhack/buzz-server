include "vocation.thrift"
include "session_event.thrift"

namespace cpp session.server.event


// 玩家登陆Session
// type: EVENT_ACTOR_LOGIN
struct EventActorLogin {
  1 : required i64 id_;
}


// 玩家登出Session
// type: EVENT_ACTOR_LOGOUT
struct EventActorLogout {
  1 : required i64 id_;
}


// 玩家数据保存
// type: EVENT_ACTOR_SAVE
struct EventActorSave {
  1 : required i64 id_;
}


// 扩展模块加载完成(每个扩展加载完成后发送的事件)
// type: EVENT_ACTOR_EXTENSION_LOAD_FINISH
struct EventActorExtensionLoadFinish {
  1 : required i64 id_;
  2 : required i32 extension_type_;
}


// 玩家数据加载完成(所有扩展加载完成后发送的事件)
// type: EVENT_ACTOR_LOAD_FINISH
struct EventActorLoadFinish {
  1 : required i64 id_;
}


// 玩家每日清理事件
// type: EVENT_ACTOR_DAILY_CLEAN
struct EventActorDailyClean {
  1 : optional i64 actor_;                          // 玩家ID
  2 : optional i32 daily_;                          // 过的天数
}


// 玩家每周清理事件
// type: EVENT_ACTOR_WEEKLY_CLEAN
struct EventActorWeeklyClean {
  1 : optional i64 actor_;                          // 玩家ID
}


// 玩家升级事件
// type: EVENT_ACTOR_LEVEL_UP
struct EventActorLevelUp {
  1 : required i64 actor_id_;                       // 玩家ID
  2 : required i32 old_level_;                      // 原等级
  3 : required i32 new_level_;                      // 新等级
}


// 玩家系统功能开启事件
// type: EVENT_ACTOR_FUNCTIONALITY_ENABLE
struct EventActorFunctionalityEnable {
  1 : required i64 actor_id_;                       // 玩家ID
  2 : required i32 functionality_id_;               // 系统功能ID
}


// 玩家切换职业事件
// type: EVENT_ACTOR_CHANGE_VOCATION
struct EventActorChangeVocation {
  1 : required i64 actor_id_;                       // 玩家ID
  2 : required vocation.VocationType src_vocation_; // 原职业
}


// 玩家战斗力变化事件
// type: EVENT_ACTOR_CHANGE_FIGHT_SCORE
struct EventActorChangeFightScore {
  1 : required i64 actor_id_;                       // 玩家ID
  2 : optional i32 old_axe_fight_score_;            // 原战士战斗力
  3 : optional i32 new_axe_fight_score_;            // 新战士战斗力
  4 : optional i32 old_shoot_fight_score_;          // 原弓手战斗力
  5 : optional i32 new_shoot_fight_score_;          // 新弓手战斗力
  6 : optional i32 old_magic_fight_score_;          // 原法师战斗力
  7 : optional i32 new_magic_fight_score_;          // 新法师战斗力
}


// 玩家跳场景登陆开始
// type: EVENT_ACTOR_JUMP_LOGIN
struct EventActorJumpLogin {
  1 : required i64 actor_;                          // 玩家ID
}


// 玩家跳场景
// type: EVENT_ACTOR_JUMP_SCENE
struct EventActorJumpScene {
  1 : required i64 actor_;                          // 玩家ID
  2 : required bool cross_game_server_;             // 是否跨 GameServer 服务器
}

