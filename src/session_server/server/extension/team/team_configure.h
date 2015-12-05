//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-09 15:44:07.
// File name: team_configure.h
//
// Description:
// Define class TeamConfigure.
//

#ifndef __SESSION__SERVER__TEAM__TEAM__CONFIGURE__H
#define __SESSION__SERVER__TEAM__TEAM__CONFIGURE__H

#include "core/base/types.h"

namespace session {

namespace server {

namespace team {

class TeamCell {
 public:
  TeamCell() : number_(0), buff_(0) {}

  // 同场景队员数量
  core::int32 number_;
  // BUFF
  core::int32 buff_;
};

}  // namespace team

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__TEAM__TEAM__CONFIGURE__H

