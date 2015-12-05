include "chat.thrift"
include "gateway_protocol.thrift"

namespace cpp gateway.protocol
namespace as3 com.dreamfire.sanguo.data.definition.protocol.gateway

/////////////////////////////////////////
// 聊天错误码
enum ChatErrorCode {
  SUCCESS = 0,                                                  // 成功
  TARGET_OFFLINE,                                               // 目标玩家不在线
  IN_TARGET_BLACKLIST,                                          // 被目标玩家拉黑
  ERROR_UNKNOWN,                                                // 未知错误
}



/////////////////////////////////////////
// 聊天请求
// type: MESSAGE_CHAT_REQUEST
// dir: client->gateway
struct MessageChatRequest {
  1 : required chat.ChatChannelType channel_;                   // 聊天频道
  2 : required string message_;                                 // 聊天信息
  3 : optional string target_name_;                             // 私聊目标玩家名
}



/////////////////////////////////////////
// 聊天回复
// type: MESSAGE_CHAT_RESPONSE
// dir: gateway->client
struct MessageChatResponse {
  1 : required ChatErrorCode result_;                           // 结果
  2 : optional string target_id_;                               // 私聊目标玩家ID
  3 : optional string target_name_;                             // 私聊目标玩家名
  4 : optional string message_;                                 // 聊天信息
}



/////////////////////////////////////////
// 聊天同步 
// type: MESSAGE_CHAT_SYNCHRONIZE
// dir: gateway->client
struct MessageChatSynchronize {
  1 : required chat.ChatChannelType channel_;                   // 聊天频道
  2 : required string message_;                                 // 聊天信息
  3 : required string actor_id_;                                // 玩家ID
  4 : required string actor_name_;                              // 玩家名
}

