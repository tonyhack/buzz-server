//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-18 16:13:50.
// File name: team_request_handler.h
//
// Description:
// Define class TeamRequestHandler.
//

#ifndef __SESSION__SERVER__TEAM__TEAM__REQUEST__HANDLER__H
#define __SESSION__SERVER__TEAM__TEAM__REQUEST__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace session {

namespace server {

namespace team {

class TeamRequestHandler : public core::Noncopyable {
 public:
  TeamRequestHandler();
  ~TeamRequestHandler();

  bool Initialize();
  void Finalize();

 private:
  int OnRequestTeamGetMembers(void *message, size_t size);
  int OnRequestTeamActorQuit(void *message, size_t size);
  int OnRequestTeamCreate(void *message, size_t size);
};

}  // namespace team

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__TEAM__TEAM__REQUEST__HANDLER__H

