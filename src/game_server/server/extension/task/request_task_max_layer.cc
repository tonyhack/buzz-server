//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-12-29 19:46:25.
// File name: request_task_max_layer.cc
//
// Description:
//
#include "game_server/server/extension/task/request_task_max_layer.h"

#include <boost/bind.hpp>

#include "entity/constants_constants.h"
#include "game_server/server/error_code.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/task/task_actor.h"
#include "game_server/server/extension/task/task_actor_manager.h"
#include "game_server/server/request/game_request_types.h"
#include "game_server/server/request/game_task_request_types.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace task {

RequestTaskMaxLayer::RequestTaskMaxLayer() {}
RequestTaskMaxLayer::~RequestTaskMaxLayer() {}

bool RequestTaskMaxLayer::Initialize() {
  // REQUEST_TASK_MAX_LAYER
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_TASK_MAX_LAYER,
      boost::bind(&RequestTaskMaxLayer::OnRequest, this, _1, _2));

  return true;
}

void RequestTaskMaxLayer::Finalize() {
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_TASK_MAX_LAYER);
}

int RequestTaskMaxLayer::OnRequest(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestTaskMaxLayer) != size) {
    global::LogError("%s:%d (%s) 请求增加道具时，参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  request::RequestTaskMaxLayer *request = (request::RequestTaskMaxLayer *)message;

  // 得到玩家任务
  TaskActor *task_actor = TaskActorManager::GetInstance()->Get(request->actor_id_);
  if(task_actor == NULL) {
    LOG_ERROR("玩家的任务对象没有找到");
    return -1;
  }

  // 得到最大的纹章id
  request->__set_task_layer_(task_actor->GetMaxLayerID());
  return 0;
}

}  // namespace task

}  // namespace server

}  // namespace game

