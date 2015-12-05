namespace cpp login.protocol
namespace as3 com.dreamfire.sanguo.data.definition.protocol.login

enum MessageType {
  MESSAGE_MIN = 0,

  MESSAGE_CREATE_ACTOR_REQUEST,             // 角色创建请求
  MESSAGE_CREATE_ACTOR_RESPONSE,            // 角色创建回应

  MESSAGE_ACTOR_LOGIN_REQUEST,              // 角色登陆请求(包含验证)
  MESSAGE_ACTOR_LOGIN_RESPONSE,             // 角色登陆回应

  MESSAGE_CHECK_ACTOR_EXIST_REQUEST,        // 检测角色是否存在请求
  MESSAGE_CHECK_ACTOR_EXIST_RESPONSE,       // 检测角色是否存在回应

  MESSAGE_MAX,
}


struct Message {
  1 : required MessageType type_;
  2 : required binary message_;
}



// type: MESSAGE_CREATE_ACTOR_REQUEST
// dir: other->login
struct MessageCreateActorRequest {
  1 : required string open_id_;             // 平台ID
  2 : required string open_session_;        // 平台Session
  3 : required i16 zone_id_;                // 区ID
  4 : required string actor_name_;          // 玩家名
  5 : required byte hair_;                  // 玩家发型
  6 : required bool male_;                  // 玩家性别
  7 : required byte vocation_;              // 玩家职业(0:近战, 1:远程, 2:法术)
}


enum CreateActorType {
  SUCCESS = 0,          // 成功
  FAILED_ACTOR_EXIST,   // 角色已存在
  FAILED_NAME_EXIST,    // 角色名重复
  FAILED_NAME_LENGTH,   // 角色名长度错误
  FAILED_NAME_INVALID,  // 角色名非法
  FAILED_UNKNOWN,       // 未知错误

  MAX,
}

// type: MESSAGE_CREATE_ACTOR_RESPONSE
// dir: login->other
struct MessageCreateActorResponse {
  1 : required CreateActorType type_;
}



// type: MESSAGE_ACTOR_LOGIN_REQUEST
// dir: other->login
struct MessageActorLoginRequest {
  1 : required string open_id_;
  2 : required i16 zone_id_;
}


// type: MESSAGE_ACTOR_LOGIN_RESPONSE
// dir: login->other
struct MessageActorLoginResponse {
  1 : required bool success_;
  2 : required string actor_id_;
  3 : required string game_session_;
  4 : required string actor_name_;
  5 : required string gateway_host_;
  6 : required i16 gateway_port_;
}


// type: MESSAGE_CHECK_ACTOR_EXIST_REQUEST
// dir: other->login
struct MessageCheckActorExistRequest {
  1 : required string open_id_;
  2 : required i16 zone_id_;
}


// type: MESSAGE_CHECK_ACTOR_EXIST_RESPONSE
// dir: login->other
struct MessageCheckActorExistResponse {
  1 : required bool exist_;
}

