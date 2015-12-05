//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-27 18:51:00.
// File name: request_start_cooling.cc
//
// Description:
// Define class RequestStartCooling.
//

#include "game_server/server/extension/cooling/request_start_cooling.h"

#include <boost/bind.hpp>

#include "entity/role_types.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/cooling/cooling_role.h"
#include "game_server/server/extension/cooling/cooling_role_manager.h"
#include "game_server/server/request/game_cooling_request_types.h"
#include "game_server/server/request/game_request_types.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace cooling {

RequestStartCooling::RequestStartCooling() {}
RequestStartCooling::~RequestStartCooling() {}

bool RequestStartCooling::Initialize() {
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_START_COOLING,
      boost::bind(&RequestStartCooling::OnRequest, this, _1, _2));
  return true;
}

void RequestStartCooling::Finalize() {
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_START_COOLING);
}

int RequestStartCooling::OnRequest(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestStartCooling) != size) {
    global::LogError("%s:%d (%s) 请求开启冷却时，参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  request::RequestStartCooling *request = (request::RequestStartCooling *)message;

  // 获取 CoolingRole 对象
  CoolingRole *role = NULL;
  if(request->type_ == entity::EntityType::TYPE_ACTOR) {
    role = CoolingRoleManager::GetInstance()->GetActor(request->id_);
  } else if(request->type_ == entity::EntityType::TYPE_NPC) {
    role = CoolingRoleManager::GetInstance()->GetNpc(request->id_);
  } else {
    global::LogError("%s:%d (%s) 请求开启冷却时，类型错误",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  // 检测是否存在
  if(role == NULL) {
    global::LogError("%s:%d (%s) 请求开启冷却时，Role(%d, %lu) 不存在",
        __FILE__, __LINE__, __FUNCTION__, request->type_, request->id_);
    return -1;
  }

  // 开启冷却
  role->StartCooling(request->cooling_id_, 0, request->cooling_add_value_,
      request->cooling_add_percent_);

  return 0;
}

}  // namespace cooling

}  // namespace server

}  // namespace game


