include "gender.thrift"
include "vocation.thrift"
include "session_event.thrift"

namespace cpp session.server.event


// 联系人信息更新事件
// type: EVENT_SOCIAL_CONTACTS_INFO_UPDATE
struct EventSocialContactsInfoUpdate {
  1 : required i64 actor_id_;                     // 玩家ID
  2 : optional bool online_;                      // 是否在线
  3 : optional i32 level_;                        // 玩家等级
  4 : optional gender.GenderType gender_;         // 玩家性别
  5 : optional vocation.VocationType vocation_;   // 玩家职业
  6 : optional i16 map_;                          // 玩家所在地图
  7 : optional string last_logout_time_;          // 上次下线时间
  8 : optional string signature_;                 // 签名
}


// 仇人信息更新事件
// type: EVENT_SOCIAL_ENEMY_INFO_UPDATE
struct EventSocialEnemyInfoUpdate {
  1 : required i64 actor_id_;                     // 玩家ID
  2 : optional bool online_;                      // 是否在线
  3 : optional i32 level_;                        // 玩家等级
  4 : optional gender.GenderType gender_;         // 玩家性别
  5 : optional vocation.VocationType vocation_;   // 玩家职业
  6 : optional i16 map_;                          // 玩家所在地图
}


// 家园好友信息更新
// type: EVENT_HOMELAND_FRIEND_INFO_UPDATE
struct EventHomelandFriendInfoUpdate {
  1 : required i64 actor_id_;                     // 玩家ID
  2 : optional bool can_visit_;                   // 是否可访问
  3 : optional bool tree_can_water_;              // 神树是否可浇水
  4 : optional bool tree_can_steal_;              // 神树是否可抢收
}


// 家园侵入者信息更新事件
// type: EVENT_HOMELAND_INVADER_INFO_UPDATE
struct EventHomelandInvaderInfoUpdate {
  1 : required i64 actor_id_;                     // 玩家ID
  2 : optional i32 level_;                        // 玩家等级
  3 : optional bool can_visit_;                   // 是否可访问
  4 : optional bool can_strike_back_;             // 是否可反击
}

