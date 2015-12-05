#include "session_server/server/extension/gm/gm_extension.h"

#include <cstddef>

#include "global/logging.h"

namespace session {

namespace server {

namespace gm {

GmExtension::GmExtension() {}
GmExtension::~GmExtension() {}

bool GmExtension::Initialize() {
  // 消息回调初始化
  if (this->game_gm_message_handler_.Initialize() == false) {
    LOG_ERROR("Init GameGmMessageHandler failed.");
    return false;
  }

  return true;
}

void GmExtension::Finalize() {
  this->game_gm_message_handler_.Finalize();
}

void GmExtension::Start() {}
void GmExtension::Stop() {}

}  // namespace gm

}  // namespace server

}  // namespace session

