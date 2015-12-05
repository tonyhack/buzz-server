//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-06-06 03:03:59.
// File name: request_ai_handler.cc
//
// Description:
// Define class RequestAiHandler.
//

#include "game_server/server/extension/ai/request_ai_handler.h"

#include <boost/bind.hpp>

#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/ai/black_board.h"
#include "game_server/server/extension/ai/black_board_manager.h"
#include "game_server/server/extension/ai/ai_actor.h"
#include "game_server/server/extension/ai/ai_actor_manager.h"
#include "game_server/server/request/game_ai_request_types.h"
#include "game_server/server/request/game_request_types.h"
#include "global/global_packet.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

RequestAiHandler::RequestAiHandler() {}
RequestAiHandler::~RequestAiHandler() {}

bool RequestAiHandler::Initialize() {
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_AI_SET_TARGET,
      boost::bind(&RequestAiHandler::OnRequestAiSetTarget, this, _1, _2));

  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_AI_SET_AI,
      boost::bind(&RequestAiHandler::OnRequestAiSetAi, this, _1, _2));

  return true;
}

void RequestAiHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_AI_SET_TARGET);

  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_AI_SET_AI);
}

int RequestAiHandler::OnRequestAiSetTarget(void *message, size_t size) {
  if (message == NULL || sizeof(request::RequestAiSetTarget) != size) {
    LOG_ERROR("参数错误");
    return -1;
  }

  request::RequestAiSetTarget *request = (request::RequestAiSetTarget *)message;

  if (request->type_ == entity::EntityType::TYPE_NPC) {
    BlackBoard *black_board = BlackBoardManager::GetInstance()->GetNpc(request->id_);
    if (black_board == NULL) {
      LOG_ERROR("获取 BlackBoard(%d,%lu) 失败", request->type_, request->id_);
      return -1;
    }
    black_board->SetForceLockTarget(false);
    black_board->SetTarget(request->target_type_, request->target_id_);
    black_board->SetForceLockTarget(request->force_lock_);

    return 0;
  }

  return -1;
}

int RequestAiHandler::OnRequestAiSetAi(void *message, size_t size) {
  if (message == NULL || sizeof(request::RequestAiSetAi) != size) {
    LOG_ERROR("参数错误");
    return -1;
  }

  request::RequestAiSetAi *request = (request::RequestAiSetAi *)message;

  if (request->type_ == entity::EntityType::TYPE_NPC) {
    BlackBoard *black_board = BlackBoardManager::GetInstance()->GetNpc(request->id_);
    if (black_board == NULL) {
      LOG_ERROR("获取 BlackBoard(%d,%lu) 失败", request->type_, request->id_);
      return -1;
    }

    black_board->SetDynamicAi(request->ai_);

    return 0;
  }

  return -1;
}

}  // namespace ai

}  // namespace server

}  // namespace game

