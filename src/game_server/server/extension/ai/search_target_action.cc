//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-17 16:29:17.
// File name: search_target_action.cc
//
// Description:
// Define class SearchTargetAction.
//

#include "game_server/server/extension/ai/search_target_action.h"

#include "game_server/server/game_role.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/ai/ai_actor.h"
#include "game_server/server/extension/ai/ai_actor_manager.h"
#include "game_server/server/extension/ai/black_board.h"
#include "game_server/server/request/game_request_types.h"
#include "game_server/server/request/game_scene_request_types.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

const std::string SearchTargetAction::kTypeName_ = "search_target";

SearchTargetAction::SearchTargetAction() {}
SearchTargetAction::~SearchTargetAction() {}

bool SearchTargetAction::LoadConfigure(TiXmlElement *node) {
  return true;
}

void SearchTargetAction::OnEnter(BlackBoard *black_board) {}

RunningStatus::type SearchTargetAction::OnExecute(BlackBoard *black_board) {
  if(black_board == NULL) {
    return RunningStatus::ERROR;
  }

  if(black_board->CheckSearchTarget(true) == false) {
    return RunningStatus::EXECUTING;
  }

  GameRole *role = black_board->GetGameRole();
  if(role == NULL) {
    return RunningStatus::ERROR;
  }

  core::uint32 x = 0, y = 0;
  role->GetPosition(x, y);

  request::RequestSceneSelectTargets request;
  request.__set_type_(role->GetType());
  request.__set_id_(role->GetID());
  request.__set_area_type_(request::AreaType::AREA_CIRCLE);
  request.__set_max_count_(1);
  request.__set_x_(x);
  request.__set_y_(y);

  request::CircleAreaCondition condition;
  condition.__set_radius_(10);

  request.__set_cond_circle_(condition);

  core::uint32 select_mask = 0;
  select_mask |= request::SelectMaskType::ACTOR;
  select_mask |= request::SelectMaskType::CAMP_ENEMY;
  select_mask |= request::SelectMaskType::OTHER_TEAM;
  select_mask |= request::SelectMaskType::ALIVE;

  request.__set_select_mask_(select_mask);

  if(ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_SCENE_SELECT_TARGETS,
        &request, sizeof(request)) == -1) {
    return RunningStatus::ERROR;
  }

  if(request.targets_.empty()) {
    return RunningStatus::EXECUTING;
  }

  if(request.targets_[0].type_ == entity::EntityType::TYPE_ACTOR) {
    AiActor *ai_actor = AiActorManager::GetInstance()->Get(request.targets_[0].id_);
    if(ai_actor == NULL) {
      LOG_ERROR("获取 AiActor(%lu) 失败", request.targets_[0].id_);
      return RunningStatus::ERROR;
    }
    if(ai_actor->GetNpc(request.targets_[0].id_) == false) {
      ai_actor->AddNpc(black_board->GetID());
    }
    black_board->GetThreat().Add(request.targets_[0].id_, 0);
    black_board->SetTarget(entity::EntityType::TYPE_ACTOR,
        black_board->GetThreat().GetMaxThreatActor());
  }

  return RunningStatus::FINISH;
}

void SearchTargetAction::OnQuit(BlackBoard *black_board) {}

}  // namespace ai

}  // namespace server

}  // namespace game

