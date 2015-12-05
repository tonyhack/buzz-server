#include "game_server/server/extension/welfare/request_handler.h"

#include <boost/bind.hpp>

#include "entity/constants_constants.h"
#include "game_server/server/error_code.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/welfare/welfare_actor.h"
#include "game_server/server/extension/welfare/welfare_actor_manager.h"
#include "game_server/server/request/game_request_types.h"
#include "game_server/server/request/game_welfare_request_types.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace welfare {

RequestHandler::RequestHandler() {}
RequestHandler::~RequestHandler() {}

bool RequestHandler::Initialize() {
  // REQUEST_WELFARE_GET_TITLE_ID
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_WELFARE_GET_TITLE_ID,
      boost::bind(&RequestHandler::OnGetTitleIDRequest, this, _1, _2));
  return true;
}

void RequestHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_WELFARE_GET_TITLE_ID);
}

int RequestHandler::OnGetTitleIDRequest(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestWelfareGetTitleID) != size) {
    LOG_ERROR("请求称号失败");
    return -1;
  }

  request::RequestWelfareGetTitleID *request = (request::RequestWelfareGetTitleID *)message;

  // 获取玩家
  WelfareActor *actor = WelfareActorManager::GetInstance()->Get(request->actor_id_);
  if(actor == NULL) {
    LOG_ERROR("玩家没有找到");
    return -1;
  }

  AchieveActor *acheive = actor->GetAchieve();
  request->__set_title_id_(acheive->GetTitleID());
  return 0;
}

}  // namespace welfare

}  // namespace server

}  // namespace game

