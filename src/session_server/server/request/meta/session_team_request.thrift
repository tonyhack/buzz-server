include "session_request.thrift"

namespace cpp session.server.request


////////////////////////////////////
// 获取组队队友列表
// type: REQUEST_TEAM_GET_MEMBERS
struct RequestTeamGetMembers {
  1 : required i64 actor_;                      // 玩家ID   (IN)
  2 : required i64 team_;                       // 队伍ID   (IN)
  3 : required i64 captain_;                    // 队长ID   (IN)
  4 : required list<i64> members_;              // 成员列表 (OUT)
}


////////////////////////////////////
// 请求离开队列
// type: REQUEST_TEAM_ACTOR_QUIT
struct RequestTeamActorQuit {
  1 : required i64 actor_;                      // 玩家ID   (IN)
}


////////////////////////////////////
// 请求创建一个队伍
// type: REQUEST_TEAM_CREATE
struct RequestTeamCreate {
  1 : required i64 captain_;                    // 队长ID   (IN)
  2 : required list<i64> members_;              // 队员列表 (IN)
  3 : required i64 team_;                       // 队伍ID   (OUT)
}

