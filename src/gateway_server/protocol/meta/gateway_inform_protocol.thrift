include "gateway_protocol.thrift"

namespace cpp gateway.protocol
namespace as3 com.dreamfire.sanguo.data.definition.protocol.gateway

/////////////////////////////////////////
// 公告信息
// type: MESSAGE_INFORM_NOTIFY
// dir: gateway->client
struct MessageInformNotify {
  1 : required i32 id_;                                         // 公告ID
  2 : required list<string> params_;                            // 参数列表
}

