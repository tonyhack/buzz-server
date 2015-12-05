namespace cpp gateway.protocol
namespace as3 com.dreamfire.sanguo.data.definition.protocol.gateway


/////////////////////////////////////////
// 错误信息
// type: MESSAGE_ERROR_CODE
// dir: gateway->client
struct MessageErrorCode {
  1 : required i32 code_;                         // 错误代码
  2 : required list<string> params_;              // 参数列表
}

