include "server_type.thrift"

namespace cpp schedule.protocol

enum MessageType {
  MESSAGE_MIN = 0,

  MESSAGE_LOGIN_REQUEST = 0,                  // 登陆请求
  MESSAGE_LOGIN_RESPONSE,                     // 登陆回复
  MESSAGE_LOGIN_BROADCAST,                    // 登陆广播

  MESSAGE_LOGOUT_REQUEST,                     // 登出请求
  MESSAGE_LOGOUT_RESPONSE,                    // 登出回复
  MESSAGE_LOGOUT_BROADCAST,                   // 登出广播

  MESSAGE_GATEWAY_USER_NUMBER_UPLOAD,         // 网关用户数量上传
  MESSAGE_GATEWAY_USER_NUMBER_BROADCAST,      // 网关用户数量广播

  MESSAGE_MAX,
}


struct Message {
  1 : required MessageType type_;
  2 : required binary message_;
}



// type: MESSAGE_LOGIN_REQUEST
// dir: other->schedule
struct MessageLoginRequest {
  1 : required server_type.ServerSignature signature_;
}

// type: MESSAGE_LOGIN_RESPONSE
// dir: schedule->other
struct MessageLoginResponse {
  1 : required bool logined_;
  2 : required list<server_type.ServerSignature> servers_;
}

// type: MESSAGE_LOGIN_BROADCAST
// dir: schedule->other
struct MessageLoginBroadcast {
  1 : required server_type.ServerSignature signature_;
}



// type: MESSAGE_LOGOUT_REQUEST
// dir: other->schedule
struct MessageLogoutRequest {
}

// type: MESSAGE_LOGIN_RESPONSE
// dir: schedule->other
struct MessageLogoutResponse {
  1 : required bool logouted_;
}

// type: MESSAGE_LOGOUT_BROADCAST
// dir: schedule->other
struct MessageLogoutBroadcast {
  1 : required server_type.ServerType type_;
  2 : required i32 id_;
}



// type: MESSAGE_GATEWAY_USER_NUMBER_UPLOAD
// dir: other->schedule
struct MessageGatewayUserNumberUpload {
  1 : required i32 number_;
}

// type: MESSAGE_GATEWAY_USER_NUMBER_BROADCAST
// dir: schedule->other
struct MessageGatewayUserNumberBroadcast {
  1 : required i32 server_id_;
  2 : required i32 number_;
}

