//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-06-22 14:30:58.
// File name: request_scene_forbid_drop_item.cc
//
// Description:
// Define class RequestSceneForbidDropItem.
//

#include "game_server/server/extension/scene/request_scene_forbid_drop_item.h"

#include <boost/bind.hpp>

#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/scene/scene_npc.h"
#include "game_server/server/extension/scene/scene_npc_manager.h"
#include "game_server/server/request/game_request_types.h"
#include "game_server/server/request/game_scene_request_types.h"

namespace game {

namespace server {

namespace scene {

RequestSceneForbidDropItem::RequestSceneForbidDropItem() {}
RequestSceneForbidDropItem::~RequestSceneForbidDropItem() {}

bool RequestSceneForbidDropItem::Initialize() {
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_SCENE_FORBID_DROP_ITEM,
      boost::bind(&RequestSceneForbidDropItem::OnRequest, this, _1, _2));

  return true;
}

void RequestSceneForbidDropItem::Finalize() {
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_SCENE_FORBID_DROP_ITEM);
}

int RequestSceneForbidDropItem::OnRequest(void *data, size_t size) {
  if (data == NULL || sizeof(request::RequestSceneForbidDropItem) != size) {
    LOG_ERROR("参数错误");
    return -1;
  }

  request::RequestSceneForbidDropItem *request =
    (request::RequestSceneForbidDropItem *)data;

  for (size_t pos = 0; pos < request->npcs_.size(); ++pos) {
    SceneNpc *npc = SceneNpcManager::GetInstance()->Get(request->npcs_[pos]);
    if (npc) {
      npc->SetForbidDropItems(request->forbid_);
    }
  }

  return 0;
}

}  // namespace scene

}  // namespace server

}  // namespace game

