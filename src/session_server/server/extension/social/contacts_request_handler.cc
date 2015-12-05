#include "session_server/server/extension/social/contacts_request_handler.h"

#include <boost/bind.hpp>

#include "session_server/server/extension_manager.h"
#include "session_server/server/request/session_request_types.h"
#include "session_server/server/request/session_social_request_types.h"
#include "session_server/server/extension/social/social_actor.h"
#include "session_server/server/extension/social/social_actor_manager.h"

namespace session {

namespace server {

namespace social {

ContactsRequestHandler::ContactsRequestHandler() {}
ContactsRequestHandler::~ContactsRequestHandler() {}

bool ContactsRequestHandler::Initialize() {
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_SOCIAL_CHECK_IN_BLACKLIST,
      boost::bind(&ContactsRequestHandler::OnRequestSocialCheckInBlacklist, this, _1, _2));

  return true;
}

void ContactsRequestHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_SOCIAL_CHECK_IN_BLACKLIST);
}

int ContactsRequestHandler::OnRequestSocialCheckInBlacklist(void *message, size_t size) {
  if (NULL == message || size != sizeof(request::RequestSocialCheckInBlacklist)) {
    LOG_ERROR("Invalid params.");
    return -1;
  }

  request::RequestSocialCheckInBlacklist *request =
    (request::RequestSocialCheckInBlacklist *)message;

  // 获取社交玩家对象
  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(request->actor_id_);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", request->actor_id_);
    return -1;
  }

  bool result = social_actor->IsInBlacklist(request->target_actor_id_);

  request->__set_result_(result);

  return 0;
}

}  // namespace social

}  // namespace server

}  // namespace session

