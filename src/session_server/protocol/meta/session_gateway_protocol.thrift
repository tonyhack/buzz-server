include "kickout_actor_type.thrift"
include "session_protocol.thrift"

namespace cpp session.protocol

enum GatewayMessageType {
  MESSAGE_MIN = 0,

  MESSAGE_USER_LOGIN_REQUEST = 0,       // 用户登陆请求
  MESSAGE_USER_LOGIN_RESPONSE,          // 用户登陆回应

  MESSAGE_USER_LOGOUT,                  // 用户退出

  MESSAGE_KICKOUT_USER,                 // 踢出用户

  MESSAGE_USER_SWITCH_GAME_REQUEST,     // 用户切换游戏服务器请求

  MESSAGE_MAX,
}



// type: MESSAGE_USER_LOGIN_REQUEST
// dir: gateway->session
struct GateMessageUserLoginRequest {
  1 : required i64 actor_id_;
}

// type: MESSAGE_USER_LOGIN_RESPONSE
// dir: session->gateway
struct GateMessageUserLoginResponse {
  1 : required bool logined_;
  2 : required i32 game_server_;
  3 : required i64 actor_id_;
}



// type: MESSAGE_USER_LOGOUT
// dir: gateway->session, session->gateway
struct GateMessageUserLogout {
  1 : required i64 actor_id_;
}



// type: MESSAGE_KICKOUT_USER
// dir: gateway->session, session->gateway
struct GateMessageKickoutUser {
  1 : required i64 actor_id_;
  2 : required kickout_actor_type.KickoutActorType type_;
}



// type: MESSAGE_USER_SWITCH_GAME_REQUEST
// dir: session->gateway
struct GateMessageUserSwitchGameRequest {
  1 : required i64 actor_;
  2 : required i32 game_server_;
}

