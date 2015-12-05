//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-10-23 13:04:37.
// File name: team_request_handler.h
//
// Description:
// Define class TeamRequestHandler.
//

#ifndef __GAME__SERVER__TEAM__TEAM__REQUEST__HANDLER__H
#define __GAME__SERVER__TEAM__TEAM__REQUEST__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace team {

class TeamRequestHandler : public core::Noncopyable {
 public:
  TeamRequestHandler();
  ~TeamRequestHandler();

  bool Initialize();
  void Finalize();

 private:
  int OnRequestTeamGetActorTeam(void *message, size_t size);
};

}  // namespace team

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TEAM__TEAM__REQUEST__HANDLER__H

