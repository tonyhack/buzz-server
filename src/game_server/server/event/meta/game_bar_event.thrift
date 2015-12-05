include "bar.thrift"

namespace cpp game.server.event
namespace py ccevent.bar


// 进度条完成事件
// type: EVENT_BAR_FINISH
struct EventBarFinish {
  1 : required i64 actor_id_;               // 玩家ID
  2 : required bar.BarModuleType type_;     // 进度条类型
}


