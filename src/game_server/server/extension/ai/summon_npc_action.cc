//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-06 15:55:28.
// File name: summon_npc_action.cc
//
// Description:
// Define class SummonNpcAction.
//

#include "game_server/server/extension/ai/summon_npc_action.h"

#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/ai/black_board.h"
#include "game_server/server/request/game_scene_request_types.h"
#include "global/logging.h" 

namespace game {

namespace server {

namespace ai {

const std::string SummonNpcAction::kTypeName_ = "summon_npc";

SummonNpcAction::SummonNpcAction() {}
SummonNpcAction::~SummonNpcAction() {}

bool SummonNpcAction::LoadConfigure(TiXmlElement *node) {
  if(node == NULL) {
    LOG_ERROR("参数错误");
    return false;
  }

  if(node->Attribute("param1", (int *)&this->npc_template_) == NULL) {
    LOG_ERROR("找不到属性[node-param1]");
    return false;
  }

  this->position_type_ = TargetType::TARGET;

  const char *str = node->Attribute("param2");
  if(str != NULL) {
    std::string position_type = str;
    if(position_type == "SELF") {
      this->position_type_ = TargetType::SELF;
    } else if(position_type == "TARGET") {
      this->position_type_ = TargetType::TARGET;
    } else {
      LOG_DEBUG("找不到属性[node-param2]");
      return false;
    }
  }

  if(node->Attribute("param3", (int *)&this->radius_) == NULL) {
    this->radius_ = 5;
  }

  return true;
}

void SummonNpcAction::OnEnter(BlackBoard *black_board) {}

RunningStatus::type SummonNpcAction::OnExecute(BlackBoard *black_board) {
  if(black_board == NULL) {
    return RunningStatus::ERROR;
  }

  GameRole *me = black_board->GetGameRole();
  if(me == NULL) {
    return RunningStatus::ERROR;
  }

  request::RequestSceneSummonNpc request;
  request.__set_template_id_(this->npc_template_);
  request.__set_scene_(me->GetScene());
  request.__set_dir_(entity::DirectionType::EAST);
  request.__set_width_(this->radius_);
  request.__set_heigth_(this->radius_);

  if(this->position_type_ == TargetType::SELF) {
    request.__set_x_(me->GetPosx());
    request.__set_y_(me->GetPosy());
  } else if(this->position_type_ == TargetType::TARGET) {
    core::uint32 x = 0, y = 0;
    black_board->GetTargetPosition(x, y);
    request.__set_x_(x);
    request.__set_y_(y);
  } else {
    return RunningStatus::ERROR;
  }

  if(ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_SCENE_SUMMON_NPC,
        &request, sizeof(request)) != 0) {
    LOG_ERROR("请求 REQUEST_SCENE_SUMMON_NPC 失败");
    return RunningStatus::ERROR;
  }

  return RunningStatus::FINISH;
}

void SummonNpcAction::OnQuit(BlackBoard *black_board) {}

}  // namespace ai

}  // namespace server

}  // namespace game

