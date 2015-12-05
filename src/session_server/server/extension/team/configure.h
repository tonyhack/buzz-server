//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-09 17:15:31.
// File name: configure.h
//
// Description:
// Define class Configure.
//

#ifndef __SESSION__SERVER__TEAM__CONFIGURE__H
#define __SESSION__SERVER__TEAM__CONFIGURE__H

#include <string>

#include "core/base/types.h"
#include "session_server/server/extension/team/team_configure.h"
#include "session_server/server/extension/team/load_team_configure.h"
#include "global/singleton_factory.h"

namespace session {

namespace server {

namespace team {

class Configure : public global::SingletonFactory<Configure> {
  friend class global::SingletonFactory<Configure>;

 public:
  bool LoadConfigure(const std::string &file);

  inline const TeamCell *GetTeamCell(core::uint32 number) {
    return this->load_team_.GetTeam(number);
  }

  int team_initial_number_;
  int team_extend_number_;

 private:
  Configure() {}
  ~Configure() {}

  LoadTeamConfigure load_team_;
};

}  // namespace team

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__TEAM__CONFIGURE__H

