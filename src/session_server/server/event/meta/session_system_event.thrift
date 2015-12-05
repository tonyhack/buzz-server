include "server_type.thrift"
include "session_event.thrift"

namespace cpp session.server.event


/////////////////////////////////
// 服务器登陆
// EVENT_SYSTEM_SERVER_LOGIN
struct EventSystemServerLogin {
  1 : required server_type.ServerType type_;          // 服务器类型
  2 : required i32 id_;                               // 服务器ID
}


/////////////////////////////////
// 服务器登出
// EVENT_SYSTEM_SERVER_LOGOUT
struct EventSystemServerLogout {
  1 : required server_type.ServerType type_;          // 服务器类型
  2 : required i32 id_;                               // 服务器ID
}


/////////////////////////////////
// 已连接到数据库服务器
// EVENT_SYSTEM_DATABASE_SERVER_LOGINED
struct EventSystemDatabaseServerLogined {
  1 : required i32 id_;                               // 服务器ID
}

