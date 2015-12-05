//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-09 11:34:35.
// File name: delete_me_action.cc
//
// Description:
// Define class DeleteMeAction.
//

#include "game_server/server/extension/ai/delete_me_action.h"

#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/ai/black_board.h"
#include "game_server/server/request/game_scene_request_types.h"
#include "global/logging.h" 

namespace game {

namespace server {

namespace ai {

const std::string DeleteMeAction::kTypeName_ = "delete_me";

DeleteMeAction::DeleteMeAction() {}
DeleteMeAction::~DeleteMeAction() {}

bool DeleteMeAction::LoadConfigure(TiXmlElement *node) {
  return true;
}

void DeleteMeAction::OnEnter(BlackBoard *black_board) {}

RunningStatus::type DeleteMeAction::OnExecute(BlackBoard *black_board) {
  if(black_board == NULL) {
    return RunningStatus::ERROR;
  }

  if(black_board->GetType() != entity::EntityType::TYPE_NPC) {
    return RunningStatus::ERROR;
  }

  request::RequestSceneClearNpc request;
  request.__set_id_(black_board->GetID());
  request.__set_delay_secs_(1);
  if(ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_SCENE_CLEAR_NPC,
        &request, sizeof(request)) != 0) {
    LOG_ERROR("请求 REQUEST_SCENE_CLEAR_NPC 失败");
    return RunningStatus::ERROR;
  }

  return RunningStatus::FINISH;
}

void DeleteMeAction::OnQuit(BlackBoard *black_board) {}

}  // namespace ai

}  // namespace server

}  // namespace game

