include "bar.thrift"
include "game_request.thrift"

namespace cpp game.server.request
namespace py ccrequest.bar

////////////////////////////
// 请求安装进度条
// type:REQUEST_BAR_START
struct RequestBarStart {
  1 : required i64 actor_id_;
  2 : required i32 bar_id_;
  3 : required bar.BarModuleType type_;
}

/////////////////////////////
// 检查进度条
// type:REQUEST_BAR_CHECK
struct RequestBarCheck {
  1 : required i32 bar_id_;
  2 : required i64 actor_id_;
}

