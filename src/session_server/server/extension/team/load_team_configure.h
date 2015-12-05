//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-09 15:47:24.
// File name: load_team_configure.h
//
// Description:
// Define class LoadTeamConfigure.
//

#ifndef __SESSION__SERVER__TEAM__LOAD__TEAM__CONFIGURE__H
#define __SESSION__SERVER__TEAM__LOAD__TEAM__CONFIGURE__H

#include <string>
#include <vector>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "session_server/server/extension/team/team_configure.h"

namespace session {

namespace server {

namespace team {

class LoadTeamConfigure : public core::Noncopyable {
  typedef std::vector<TeamCell> TeamVector;

 public:
  LoadTeamConfigure();
  ~LoadTeamConfigure();

  bool LoadConfigure(const std::string &file);

  const TeamCell *GetTeam(core::uint32 number);

 private:
  TeamVector teams_;
};

}  // namespace team

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__TEAM__LOAD__TEAM__CONFIGURE__H

