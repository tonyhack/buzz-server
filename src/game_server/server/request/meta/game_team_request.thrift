include "game_request.thrift"

namespace cpp game.server.request
namespace py ccrequest.team

//////////////////////////////////
// 请求获取玩家所在组队
// type: REQUEST_TEAM_GET_ACTOR_TEAM
struct RequestTeamGetActorTeam {
  1 : required i64 actor_id_;                 // 玩家ID (IN)
  2 : required i64 team_id_;                  // 队伍ID (OUT)
}

