include "game_event.thrift"
include "actor.thrift"
include "vip.thrift"
include "resource.thrift"
include "npc.thrift"

namespace cpp game.server.event
namespace py ccevent.actor


/////////////////////////////////
// 玩家上线事件
// type: EVENT_ACTOR_LOGIN
struct EventActorLogin {
  1 : required i64 id_;
  2 : required i16 map_;
  3 : required i32 x_;
  4 : required i32 y_;
}


/////////////////////////////////
// 玩家下线事件
// type: EVENT_ACTOR_LOGOUT
struct EventActorLogout {
  1 : required i64 id_;
}


/////////////////////////////////
// 玩家保存事件
// type: EVENT_ACTOR_SAVE
struct EventActorSave {
  1 : required i64 id_;
}


/////////////////////////////////
// 玩家扩展模块加载完成
// type: EVENT_ACTOR_EXTENSION_LOAD_FINISH
struct EventActorExtensionLoadFinish {
  1 : required i64 id_;
  2 : required i32 extension_type_;
}


/////////////////////////////////
// 玩家数据加载完成
// type: EVENT_ACTOR_LOAD_FINISH
struct EventActorLoadFinish {
  1 : required i64 id_;
}


/////////////////////////////////
// 玩家每日清理事件
// type: EVENT_ACTOR_DAILY_CLEAN
struct EventActorDailyClean {
  1 : optional i64 actor_;                        // 玩家ID
  2 : optional i32 days_;                         // 过的天数
}



/////////////////////////////////
// 玩家每周清理事件
// type: EVENT_ACTOR_WEEKLY_CLEAN
struct EventActorWeeklyClean {
  1 : optional i64 actor_;                        // 玩家ID
}



/////////////////////////////////
// 玩家与NPC对话事件
// type: EVENT_ACTOR_TALK_NPC
struct EventActorTalkNpc {
  1 : required i64 actor_id_;                     // 玩家ID
  2 : required i32 npc_id_;                       // NPC的ID
  3 : optional i32 task_id_ = 0;                  // 任务ID
}


/////////////////////////////////
// 玩家升级事件
// type: EVENT_ACTOR_LEVEL_UP
struct EventActorLevelUp {
  1 : required i64 actor_id_;                     // 玩家ID
  2 : required i32 old_level_;                    // 原等级
  3 : required i32 new_level_;                    // 新等级
}


/////////////////////////////////
// 玩家系统功能开启事件
// type: EVENT_ACTOR_FUNCTIONALITY_ENABLE
struct EventActorFunctionalityEnable {
  1 : required i64 actor_id_;                     // 玩家ID
  2 : required i32 functionality_id_;             // 系统功能ID
}


/////////////////////////////////
// 玩家杀死NPC事件
// type: EVENT_ACTOR_KILL_NPC
struct EventActorKillNpc {
  1 : required i64 actor_;                        // 玩家ID
  2 : required i64 npc_;                          // NPC
  3 : required i32 npc_template_;                 // NPC模板ID
  4 : required npc.NpcMobType type_;              // NPC模型
  5 : required i32 npc_level_;                    // NPC等级
}

/////////////////////////////////
// 玩家对比事件
// type: EVENT_ACTOR_COMPARE
struct EventActorCompare {
  1 : required i64 actor_;                        // 玩家ID
  2 : required i64 compare_actor_;                // 被比较者玩家ID
}


////////////////////////////////
// 玩家VIP变化事件
// type: EVENT_ACTOR_VIP_CHANGE
struct EventActorVipChange  {
  1 : required i64 actor_;                        // 玩家ID
  2 : required vip.VipType old_type_;             // 旧的VIP类型
  3 : required vip.VipType new_type_;             // 新的VIP类型
}


////////////////////////////////
// 资源发生变化事件
// type: EVENT_ACTOR_RESOURCE_CHANGE
struct EventActorResourceChange {
  1 : required i64 actor_;                        // 玩家ID
  2 : required resource.ResourceID type_;         // 资源类型
  3 : required i32 old_num_;                      // 变化前
  4 : required i32 new_num_;                      // 变化后
}


////////////////////////////////
// 复活事件
// type: EVENT_ACTOR_RELIVE
struct EventActorRelive {
  1 : required i64 actor_;
  2 : required actor.ReviveType type_;            // 复活方式
}

////////////////////////////////
// 玩家防沉迷事件
// type: EVENT_ACTOR_FCM_STATUS_CHANGE
struct EventActorFcmStatusChange {
  1 : required i64 actor_;                        // 玩家ID
}
