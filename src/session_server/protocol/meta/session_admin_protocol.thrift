namespace cpp session.protocol
namespace php Admin

enum AdminMessageType {
  MESSAGE_MIN = 0,

  MESSAGE_ADMIN_USER_NUMBER_QUERY_REQUEST,                     // 玩家人数查询请求
  MESSAGE_ADMIN_USER_NUMBER_QUERY_RESPONSE,                    // 玩家人数查询回复

  MESSAGE_MAX,
}

struct MessageAdmin {
  1 : required AdminMessageType type_;                          // 消息类型
  2 : required binary message_;                                 // 消息数据
}



/////////////////////////////////////////
// 玩家人数查询请求
// type: MESSAGE_AMDIN_USER_NUMBER_QUERY_REQUEST
struct MessageAdminUserNumberQueryRequest {
}



/////////////////////////////////////////
// 玩家人数查询回复
// type: MESSAGE_ADMIN_USER_NUMBER_QUERY_RESPONSE
struct MessageAdminUserNumberQueryResponse {
  1 : required i32 online_number_;                              // 在线人数
  2 : required i32 total_number_;                               // 总人数
}

