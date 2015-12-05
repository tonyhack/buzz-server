include "session_request.thrift"

namespace cpp session.server.request

////////////////////////////////////////////
// 公会属性类型 
enum GuildAttributeType {
  MIN = 0,

  LIGHT = 0,                                          // 光属性
  DARK,                                               // 暗属性
  HOLY,                                               // 圣属性

  MAX,
}



////////////////////////////////////////////
// 获取玩家公会ID
// type: REQUEST_GUILD_GET_ID
struct RequestGuildGetID {
  1 : required i64 actor_id_;                         // 玩家ID (IN)
  2 : required i32 guild_id_;                         // 公会ID (OUT)       
}



////////////////////////////////////////////
// 增加公会贡献
// type: REQUEST_GUILD_ADD_CONTRIBUTION
struct RequestGuildAddContribution {
  1 : required i64 actor_id_;                         // 玩家ID (IN)
  2 : required i32 contribution_;                     // 贡献值 (IN)
}



////////////////////////////////////////////
// 检查是否有公会聊天权限
// type: REQUEST_GUILD_CHECK_AUTHORITY_CHAT
struct RequestGuildCheckAuthorityChat {
  1 : required i64 actor_id_;                         // 玩家ID (IN)
  2 : required bool result_;                          // 结果   (OUT)
}



////////////////////////////////////////////
// 增加公会属性
// type: REQUEST_GUILD_ADD_ATTRIBUTE
struct RequestGuildAddAttribute {
  1 : optional i64 guild_id_;                         // 公会ID   (IN) 
  2 : optional i64 actor_id_;                         // 玩家ID   (IN)
  3 : required GuildAttributeType type_;              // 属性类型 (IN)
  4 : required i32 value_;                            // 增加值   (IN)
}



////////////////////////////////////////////
// 获取公会成员列表
// type: REQUEST_GUILD_GET_MEMBERS
struct RequestGuildGetMembers {
  1 : required i64 actor_id_;                         // 玩家ID       (IN)
  2 : required list<i64> members_;                    // 公会成员列表 (OUT)
}



////////////////////////////////////////////
// 检查是否可以创建公会副本
// type: REQUEST_GUILD_CHECK_CAN_CREATE_PLAYING
struct RequestGuildCheckCanCreatePlaying {
  1 : required i64 actor_id_;                         // 玩家ID     (IN)
  2 : required i32 playing_template_id_;              // 副本模板ID (IN)
  3 : required bool result_;                          // 结果       (OUT)
}



////////////////////////////////////////////
// 获取当前正在进行中的公会副本信息
// type: REQUEST_GUILD_GET_CURRENT_PLAYING
struct RequestGuildGetCurrentPlaying {
  1 : optional i64 guild_id_;                         // 公会ID     (IN) 
  2 : optional i64 actor_id_;                         // 玩家ID     (IN)
  3 : required i64 playing_id_;                       // 副本ID     (OUT)
  4 : required i32 playing_template_id_;              // 副本模板ID (OUT)
}



////////////////////////////////////////////
// 设置当前正在进行中的公会副本
// type: REQUEST_GUILD_SET_CURRENT_PLAYING
struct RequestGuildSetCurrentPlaying {
  1 : optional i64 guild_id_;                         // 公会ID     (IN) 
  2 : optional i64 actor_id_;                         // 玩家ID     (IN)
  3 : required i64 playing_id_;                       // 副本ID     (IN)
  4 : required i32 playing_template_id_;              // 副本模板ID (IN)
}

