include "session_request.thrift"

namespace cpp session.server.request

////////////////////////////////////////////
// 检查是否在黑名单中(目标玩家是否在玩家的黑名单中)
// type: REQUEST_SOCIAL_CHECK_IN_BLACKLIST
struct RequestSocialCheckInBlacklist {
  1 : required i64 actor_id_;                         // 玩家ID     (IN)
  2 : required i64 target_actor_id_;                  // 目标玩家ID (IN)
  3 : required bool result_;                          // 结果       (OUT)
}

