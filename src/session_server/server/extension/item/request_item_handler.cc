#include "session_server/server/extension/item/request_item_handler.h"

#include <boost/bind.hpp>
#include "session_server/protocol/session_game_protocol_types.h"
#include "session_server/server/extension_manager.h"
#include "session_server/server/session_terminal.h"
#include "session_server/server/request/session_item_request_types.h"
#include "session_server/server/extension/item/item_extension.h"
#include "global/global_packet.h"
#include "global/common_functions.h"
#include "global/logging.h"

namespace session {

namespace server {

namespace item {

RequestItemHandler::RequestItemHandler() {}
RequestItemHandler::~RequestItemHandler() {}

bool RequestItemHandler::Initialize() {
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_MAIL_SEND_MAIL,
      boost::bind(&RequestItemHandler::OnRequestMallTreasureConfigureData, this, _1, _2));
  return true;
}

void RequestItemHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetRequestHandler(
     request::RequestType::REQUEST_MAIL_SEND_MAIL);
}

int RequestItemHandler::OnRequestMallTreasureConfigureData(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestItemMallTreasureConfigureData) != size) {
    LOG_ERROR("请求增加邮件时，参数错误");
    return -1;
  }
  request::RequestItemMallTreasureConfigureData *request = 
    (request::RequestItemMallTreasureConfigureData *)message;

  ItemExtension::GetInstance()->ClearMallConfigure();
  for(size_t i = 0; i < request->configure_lists_.size(); ++i) {
    if(ItemExtension::GetInstance()->PushMallTreasureData(request->configure_lists_[i]) == false) {
      LOG_ERROR("加入的商城配置错误，已经忽略此行line(%d)", i + 1);
    }
  }
  ItemExtension::GetInstance()->SynchronizeMallTreasureData();
  return 0;
}

} // namespace item

} // namespace server 

} // namespace session
