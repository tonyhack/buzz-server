#include "session_server/server/extension/social/facade_request.h"

#include "session_server/server/extension_manager.h"
#include "session_server/server/request/session_request_types.h"
#include "session_server/server/request/session_guild_request_types.h"

namespace session {

namespace server {

namespace social {

core::uint64 FacadeRequest::GetGuildID(core::uint64 actor_id) {
  request::RequestGuildGetID request;
  request.__set_actor_id_(actor_id);

  if (ExtensionManager::GetInstance()->Request(
          request::RequestType::REQUEST_GUILD_GET_ID,
          &request, sizeof(request)) != 0)  {
    LOG_ERROR("Get guild ID failed.");
    return 0;
  } else {
    return request.guild_id_;
  }
}

bool FacadeRequest::CheckInSameGuild(core::uint64 actor1_id, core::uint64 actor2_id) {
  core::uint64 guild1_id = this->GetGuildID(actor1_id);
  core::uint64 guild2_id = this->GetGuildID(actor2_id);

  if (0 == guild1_id || 0 == guild2_id) {
    return false;
  }

  return guild1_id == guild2_id;
}

void FacadeRequest::AddGuildContribution(core::uint64 actor_id, core::int32 contribution) {
  request::RequestGuildAddContribution request;
  request.__set_actor_id_(actor_id);
  request.__set_contribution_(contribution);

  if (ExtensionManager::GetInstance()->Request(
      request::RequestType::REQUEST_GUILD_ADD_CONTRIBUTION,
      &request, sizeof(request)) != 0) {
    LOG_ERROR("Add guild contribution failed.");
  }
}

}  // namespace social

}  // namespace server

}  // namespace session

