//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-08-22 15:32:39.
// File name: team_message_handler.h
//
// Description:
// Define class TeamMessageHandler.
//

#ifndef __SESSION__SERVER__TEAM__TEAM__MESSAGE__HANDLER__H
#define __SESSION__SERVER__TEAM__TEAM__MESSAGE__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "gateway_server/protocol/gateway_team_protocol_types.h"
#include "session_server/server/request/session_scene_request_types.h"

namespace session {

namespace server {

namespace team {

class TeamMessageHandler : public core::Noncopyable {
 public:
  TeamMessageHandler();
  ~TeamMessageHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnMessageTeamCreateRequest(core::uint64 id, const char *data, size_t size);
  void OnMessageTeamJoinRequest(core::uint64 id, const char *data, size_t size);
  void OnMessageTeamJoinDisposeRequest(core::uint64 id, const char *data, size_t size);
  void OnMessageTeamInviteRequest(core::uint64 id, const char *data, size_t size);
  void OnMessageTeamInviteDisposeRequest(core::uint64 id, const char *data, size_t size);
  void OnMessageTeamDismissRequest(core::uint64 id, const char *data, size_t size);
  void OnMessageTeamQuitRequest(core::uint64 id, const char *data, size_t size);
  void OnMessageTeamKickoutRequest(core::uint64 id, const char *data, size_t size);
  void OnMessageTeamSetCaptainRequest(core::uint64 id, const char *data, size_t size);
  void OnMessageTeamMapTeamsRequest(core::uint64 id, const char *data, size_t size);
  void OnMessageTeamMapActorsRequest(core::uint64 id, const char *data, size_t size);
  void OnMessageTeamMemberRequest(core::uint64 id, const char *data, size_t size);

  request::RequestSceneGetActors request_scene_get_actors_;

  gateway::protocol::MessageTeamMapTeamsResponse message_team_map_teams_;
  gateway::protocol::MessageTeamMapActorsResponse message_team_map_actors_;
};

}  // namespace team

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__TEAM__TEAM__MESSAGE__HANDLER__H

