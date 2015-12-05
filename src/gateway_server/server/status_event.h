//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-04 12:02:16.
// File name: status_event.h
//
// Description:
// Define status event.
//

#ifndef __GATEWAY__SERVER__STATUS__EVENT__H
#define __GATEWAY__SERVER__STATUS__EVENT__H

namespace gateway {

namespace server {

struct StatusEventType {
  enum type {
    EVENT_CHECK_SESSION = 0,      // 检测session与登陆状态
    EVENT_USER_ONLINE,            // 玩家上线
    EVENT_USER_LOGIN,             // 玩家登陆

    EVENT_MAX,
  };
};

// EVENT_CHECK_SESSION
struct StatusEventCheckSession {
  bool result_;     // 检测结果
  bool online_;     // 在线状态
  time_t logout_time_;  // 上次下线时间
};

// EVENT_USER_ONLINE
struct StatusEventUserOnline {
  core::int8 online_;   // 之前在线状态
  time_t logout_time_;  // 上次下线时间
};

// EVENT_USER_LOGIN
struct StatusEventUserLogin {
  bool result_;     // 登陆结果
};

}  // namespace server

}  // namespace gateway

#endif  // __GATEWAY__SERVER__STATUS__EVENT__H

