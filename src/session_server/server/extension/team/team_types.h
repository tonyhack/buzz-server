//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-02 16:05:01.
// File name: team_types.h
//
// Description:
// Define team types.
//

#ifndef __SESSION__SERVER__TEAM__TEAM__TYPES__H
#define __SESSION__SERVER__TEAM__TEAM__TYPES__H

namespace session {

namespace server {

namespace team {

struct TeamCommandType {
  enum type {
    MIN = 0,

    JOIN = 0,            // 申请
    INVITE,              // 邀请

    MAX,
  };
};

}  // namespace team

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__TEAM__TEAM__TYPES__H

