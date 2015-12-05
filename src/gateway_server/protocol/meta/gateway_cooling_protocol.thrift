include "gateway_protocol.thrift"
include "cooling.thrift"

namespace cpp gateway.protocol
namespace as3 com.dreamfire.sanguo.data.definition.protocol.gateway


//////////////////////////////////////
// 冷却初始化
// type: MESSAGE_COOLING_INITIALIZE
// dir: gateway->client
struct MessageCoolingInitialize {
  1 : required list<cooling.CoolingFields> coolings_;         // 冷却数据列表
}

