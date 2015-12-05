//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-23 09:58:34.
// File name: gm_extension.cc
//
// Description:
// Define class GmExtension.
//

#include "game_server/server/extension/gm/gm_extension.h"

#include "global/logging.h"

namespace game {

namespace server {

namespace gm {

GmExtension::GmExtension() {}
GmExtension::~GmExtension() {}

bool GmExtension::Initialize() {
  // 消息回调
  if(this->gm_message_handler_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 GmMessageHandler 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  return true;
}

void GmExtension::Finalize() {
  this->gm_message_handler_.Finalize();
}

void GmExtension::Start() {}

void GmExtension::Stop() {}

}  // namespace gm

}  // namespace server

}  // namespace game

