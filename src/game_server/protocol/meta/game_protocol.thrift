include "server_type.thrift"

namespace cpp game.protocol

enum MessageType {
  MESSAGE_MIN = 0,

  MESSAGE_LOGIN_REQUEST = 0,                  // 登陆请求
  MESSAGE_LOGIN_RESPONSE,                     // 登陆回复

  MESSAGE_LOGOUT_REQUEST,                     // 登出请求
  MESSAGE_LOGOUT_RESPONSE,                    // 登出回复

  MESSAGE_USER_MESSAGE_FORWARD,               // 用户消息转发

  MESSAGE_USER_MESSAGE_BROADCAST,             // 用户消息广播(游戏服务器到网关)
  MESSAGE_USER_MESSAGE_MULTICAST,             // 用户消息多播(游戏服务器到网关)

  MESSAGE_USER_LOGOUT,                        // 用户退出

  MESSAGE_MAX,
}

struct Message {
  1 : required MessageType type_;
  2 : required binary message_;
}



// type: MESSAGE_LOGIN_REQUEST
// dir: other->game
struct MessageLoginRequest {
  1 : required server_type.ServerSignature signature_;
}

// type: MESSAGE_LOGIN_RESPONSE
// dir: game->other
struct MessageLoginResponse {
  1 : required bool logined_;
}



// type: MESSAGE_LOGOUT_REQUEST
// dir: other->game
struct MessageLogoutRequest {
}

// type: MESSAGE_LOGIN_RESPONSE
// dir: game->other
struct MessageLogoutResponse {
  1 : required bool logouted_;
}



// type: MESSAGE_USER_MESSAGE_FORWARD
// dir: game->other && other->game
struct MessageUserMessageForward {
  1 : required binary message_;
  2 : required i32 type_;
  3 : required i64 actor_;
}



// type: MESSAGE_USER_MESSAGE_BROADCAST
// dir: game->other(gateway)
struct MessageUserMessageBroadcast {
  1 : required binary message_;
  2 : required i32 type_;
}



// type: MESSAGE_USER_MESSAGE_MULTICAST
// dir: game->other(gateway)
struct MessageUserMessageMulticast {
  1 : required binary message_;
  2 : required i32 type_;
  3 : required list<i64> actors_;
}



// type: MESSAGE_USER_LOGOUT
struct MessageUserLogout {
  1 : required i64 actor_id_;
}

