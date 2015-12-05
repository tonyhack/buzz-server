#include "session_server/server/extension/chat/facade_request.h"

#include "session_server/server/extension_manager.h"
#include "session_server/server/request/session_request_types.h"
#include "session_server/server/request/session_social_request_types.h"
#include "session_server/server/request/session_guild_request_types.h"

namespace session {

namespace server {

namespace chat {

bool FacadeRequest::CheckInBlacklist(core::uint64 actor_id, core::uint64 target_actor_id) {
  request::RequestSocialCheckInBlacklist request;
  request.__set_actor_id_(actor_id);
  request.__set_target_actor_id_(target_actor_id);
  if (ExtensionManager::GetInstance()->Request(
          request::RequestType::REQUEST_SOCIAL_CHECK_IN_BLACKLIST, &request,
          sizeof(request)) == 0) {
    return request.result_;
  }
  return false;
}

bool FacadeRequest::CheckGuildAuthorityChat(core::uint64 actor_id) {
  request::RequestGuildCheckAuthorityChat request;
  request.__set_actor_id_(actor_id);
  if (ExtensionManager::GetInstance()->Request(
          request::RequestType::REQUEST_GUILD_CHECK_AUTHORITY_CHAT, &request,
          sizeof(request)) == 0) {
    return request.result_;
  }
  return false;
}

}  // namespace chat

}  // namespace server

}  // namespace session

