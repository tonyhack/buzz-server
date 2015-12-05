include "bar.thrift"
include "gateway_protocol.thrift"


namespace cpp gateway.protocol
namespace as3 com.dreamfire.sanguo.data.definition.protocol.gateway



/////////////////////////////////////////////
// 进度条打断
// type: MESSAGE_BAR_INTERRUPT
// dir: gateway->client
struct MessageBarInterrupt {
  1 : required bar.BarEventType interrupt_type_;         // 打断类型
}

///////////////////////////////////////////
// 请求完成进度条
// type: MESSAGE_BAR_FINISH_REUEST
// dir:  client->gateway
struct MessageBarFinishRequest {
  1 : required i32 bar_id_;                              // 请求完成进度条
}

//////////////////////////////////////////
// 请求完成进度条响应
// type: MESSAGE_BAR_FINISH_RESPONSE
// dir:  gateway->client
struct MessageBarFinishResponse {
  1 : required bar.BarModuleType type_;                  // 进度条所属的子类型
}

////////////////////////////////////////////
// 安装进度条
// type: MESSAGE_BAR_START_SYNCHRONIZE
// dir:  gateway->client
struct MessageBarStartSynchoronize {
  1 : required i32 bar_id_;                              // 进度条id
}

