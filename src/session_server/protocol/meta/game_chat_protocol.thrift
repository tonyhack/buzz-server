include "chat.thrift"
include "session_game_protocol.thrift"

namespace cpp session.protocol



////////////////////////////////////////
// 聊天请求
// type: MESSAGE_CHAT_REQUEST
// dir: game->session
struct MessageChatRequest {
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : required chat.ChatChannelType channel_;                   // 聊天频道
  3 : required string message_;                                 // 聊天信息
  4 : optional string target_name_;                             // 私聊目标玩家名
}
