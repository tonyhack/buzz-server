include "session_game_protocol.thrift"

namespace cpp session.protocol



////////////////////////////////////////
// 通用GM指令
// type: MESSAGE_GM_COMMAND
// dir: game->session
struct MessageGmCommand {
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : required string command_;                                 // GM指令
}

