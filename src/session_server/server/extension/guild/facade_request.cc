#include "session_server/server/extension/guild/facade_request.h"

#include "entity/mail_system_types.h"
#include "session_server/server/extension_manager.h"
#include "session_server/server/request/session_request_types.h"
#include "session_server/server/request/session_mail_request_types.h"

namespace session {

namespace server {

namespace guild {

bool FacadeRequest::SendDismissMail(core::uint64 actor_id) {
  request::RequestSendMail request;
  request.__set_id_(actor_id);
  request.__set_child_type_(entity::SystemMailChildType::GUILD_DISMISS);
  if (ExtensionManager::GetInstance()->Request(
          request::RequestType::REQUEST_MAIL_SEND_MAIL, &request,
          sizeof(request)) == 0) {
    return true;
  }
  return false;
}

}  // namespace guild

}  // namespace server

}  // namespace session

