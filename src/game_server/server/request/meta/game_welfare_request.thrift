include "entity.thrift"
include "game_request.thrift"

namespace cpp game.server.request
namespace py ccrequest.welfare

///////////////////////////////////
// 请求玩家称号ID
// type: REQUEST_WELFARE_GET_TITLE_ID
struct RequestWelfareGetTitleID {
  1 : required i64 actor_id_;                 // 玩家ID
  2 : required i32 title_id_;                 // 称号ID(成就ID)
}
