#include "session_server/server/admin/admin_message_handler.h"

#include <boost/bind.hpp>

#include "global/global_packet.h"
#include "global/logging.h"
#include "session_server/server/session_actor_manager.h"
#include "session_server/server/admin/admin_service.h"
#include "session_server/server/admin/admin_session.h"

namespace session {

namespace server {

namespace admin {
AdminMessageHandler::AdminMessageHandler() {}
AdminMessageHandler::~AdminMessageHandler() {}

bool AdminMessageHandler::Initialize() {
  AdminService::GetInstance()->SetMessageHandler(
    session::protocol::AdminMessageType::MESSAGE_ADMIN_USER_NUMBER_QUERY_REQUEST,
    boost::bind(&AdminMessageHandler::OnMessageAdminUserNumberQueryRequest,
        this, _1, _2, _3));

  return true;
}

void AdminMessageHandler::Finalize() {
  AdminService::GetInstance()->ResetMessageHandler(
    session::protocol::AdminMessageType::MESSAGE_ADMIN_USER_NUMBER_QUERY_REQUEST);
}

void AdminMessageHandler::OnMessageAdminUserNumberQueryRequest(AdminSession *session,
    const char *data, size_t size) {
  if (NULL == session || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageAdminUserNumberQueryRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageAdminUserNumberQueryRequest failed.");
    return;
  }

  LOG_INFO("Admin: MESSAGE_AMDIN_USER_NUMBER_QUERY_REQUEST.");

  session::protocol::MessageAdminUserNumberQueryResponse response;
  response.__set_online_number_(
      SessionActorManager::GetInstance()->GetOnlineUserNumber());
  response.__set_total_number_(
      SessionActorManager::GetInstance()->GetTotalUserNumber());
  session->SendMessage(response,
      session::protocol::AdminMessageType::MESSAGE_ADMIN_USER_NUMBER_QUERY_RESPONSE);
}

}  // namespace admin

}  // namespace server

}  // namespace session

