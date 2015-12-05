include "game_request.thrift"

namespace cpp game.server.request
namespace py ccrequest.guild

//////////////////////////////////
// 请求获取玩家所在公会
// type: REQUEST_GUILD_GET_ACTOR_GUILD
struct RequestGuildGetActorGuild {
  1 : required i64 actor_id_;                 // 玩家ID (IN)
  2 : required i64 guild_id_;                 // 公会ID (OUT)
}

//////////////////////////////////
// 请求获取玩家所在公会名
// type: REQUEST_GUILD_GET_ACTOR_GUILD_NAME
struct RequestGuildGetActorGuildName {
  1 : required i64 actor_id_;                 // 玩家ID   (IN)
  2 : required string guild_name_;            // 公会名称 (OUT)
}

