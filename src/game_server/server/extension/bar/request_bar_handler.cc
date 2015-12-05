//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Ebar: lxf@gbar.com.
// Last modify: 2013-11-04 03:58:42.
// File name: request_bar_handler.cc
//
// Description:
//
#include "game_server/server/extension/bar/request_bar_handler.h"

#include <boost/bind.hpp>

#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/bar/bar_actor.h"
#include "game_server/server/extension/bar/bar_actor_manager.h"
#include "game_server/server/request/game_request_types.h"
#include "game_server/server/request/game_bar_request_types.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace bar {

RequestBarHandler::RequestBarHandler() {}
RequestBarHandler::~RequestBarHandler() {}

bool RequestBarHandler::Initialize() {
  //  REQUEST_BAR_START
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_BAR_START,
      boost::bind(&RequestBarHandler::OnRequestBarStart, this, _1, _2));

  // REQUEST_BAR_CHECK 
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_BAR_CHECK,
      boost::bind(&RequestBarHandler::OnRequestBarCheck, this, _1, _2));
  return true;
}

void RequestBarHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_BAR_START);
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_BAR_CHECK);
}

int RequestBarHandler::OnRequestBarStart(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestBarStart) != size) {
    LOG_ERROR("请求增加进度条时，参数错误");
    return -1;
  }

  request::RequestBarStart *request = (request::RequestBarStart *)message;

  // 找到玩家
  BarActor *bar_actor = BarActorManager::GetInstance()->Get(request->actor_id_);
  if(bar_actor == NULL) {
    LOG_ERROR("玩家进度条模块没有找到[%lu]", request->bar_id_);
    return -1;
  }

  // 添加进度条
  if(bar_actor->StarBar(request->bar_id_, request->type_) == false) {
    LOG_ERROR("[%lu]开启进度条没有成功[%d]", request->actor_id_, request->bar_id_);
    return -1;
  }

  return 0;
}

int RequestBarHandler::OnRequestBarCheck(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestBarCheck) != size) {
    LOG_ERROR("请求检查进度条时，参数错误");
    return -1;
  }

  request::RequestBarCheck *request = (request::RequestBarCheck *)message;

  // 找到玩家
  BarActor *bar_actor = BarActorManager::GetInstance()->Get(request->actor_id_);
  if(bar_actor == NULL) {
    LOG_ERROR("玩家进度条模块没有找到[%lu]", request->bar_id_);
    return -1;
  }

  // 添加进度条
  if(bar_actor->CheckBar(request->bar_id_) == false) {
    LOG_ERROR("[%lu]检查进度条错误[%d]", request->actor_id_, request->bar_id_);
    return -1;
  }

  return 0;
}

} // namespace bar

} // namespace server 

} // namespace game
