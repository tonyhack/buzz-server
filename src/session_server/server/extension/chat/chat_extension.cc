#include "session_server/server/extension/chat/chat_extension.h"

#include <cstddef>

#include "global/logging.h"
#include "session_server/server/extension/chat/facade_request.h"

namespace session {

namespace server {

namespace chat {

ChatExtension::ChatExtension() {}
ChatExtension::~ChatExtension() {}

bool ChatExtension::Initialize() {
  if (FacadeRequest::GetInstance() == NULL) {
    LOG_ERROR("Init FacadeRequest singleton failed.");
    return false;
  }

  // 消息回调初始化
  if (this->game_chat_message_handler_.Initialize() == false) {
    LOG_ERROR("Init ChatMessageHandler failed.");
    return false;
  }

  return true;
}

void ChatExtension::Finalize() {
  this->game_chat_message_handler_.Finalize();

  FacadeRequest::ReleaseInstance();
}

void ChatExtension::Start() {}
void ChatExtension::Stop() {}

}  // namespace chat

}  // namespace server

}  // namespace session

