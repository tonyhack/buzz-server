//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-27 18:05:22.
// File name: request_check_in_cooling.cc
//
// Description:
// Define class RequestCheckInCooling.
//

#include "game_server/server/extension/cooling/request_check_in_cooling.h"

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

RequestCheckInCooling::RequestCheckInCooling() {}
RequestCheckInCooling::~RequestCheckInCooling() {}

bool RequestCheckInCooling::Initialize() {
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_CHECK_IN_COOLING,
      boost::bind(&RequestCheckInCooling::OnRequest, this, _1, _2));
  return true;
}

void RequestCheckInCooling::Finalize() {
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_CHECK_IN_COOLING);
}

int RequestCheckInCooling::OnRequest(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestCheckInCooling) != size) {
    global::LogError("%s:%d (%s) 请求检测冷却时，参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  request::RequestCheckInCooling *request = (request::RequestCheckInCooling *)message;

  // 获取 CoolingRole 对象
  CoolingRole *role = NULL;
  if(request->type_ == entity::EntityType::TYPE_ACTOR) {
    role = CoolingRoleManager::GetInstance()->GetActor(request->id_);
  } else if(request->type_ == entity::EntityType::TYPE_NPC) {
    role = CoolingRoleManager::GetInstance()->GetNpc(request->id_);
  } else {
    global::LogError("%s:%d (%s) 请求检测冷却时，类型错误",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  // 检测是否存在
  if(role == NULL) {
    global::LogError("%s:%d (%s) 请求检测冷却时，Role(%d, %lu) 不存在",
        __FILE__, __LINE__, __FUNCTION__, request->type_, request->id_);
    return -1;
  }

  // 设置是否在冷却
  request->__set_in_cooling_(!role->CheckCoolingEnd(request->cooling_id_));

  return 0;
}

}  // namespace cooling

}  // namespace server

}  // namespace game

