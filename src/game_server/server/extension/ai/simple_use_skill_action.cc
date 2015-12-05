//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-09 12:09:33.
// File name: simple_use_skill_action.cc
//
// Description:
// Define class SimpleUseSkillAction.
//

#include "game_server/server/extension/ai/simple_use_skill_action.h"

#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/ai/black_board.h"
#include "game_server/server/request/game_skill_request_types.h"
#include "global/logging.h" 

namespace game {

namespace server {

namespace ai {

const std::string SimpleUseSkillAction::kTypeName_ = "simple_use_skill";

SimpleUseSkillAction::SimpleUseSkillAction() {}
SimpleUseSkillAction::~SimpleUseSkillAction() {}

bool SimpleUseSkillAction::LoadConfigure(TiXmlElement *node) {
  if(node == NULL) {
    LOG_ERROR("参数错误");
    return false;
  }

  if(node->Attribute("param1", (int *)&this->skill_id_) == NULL) {
    this->skill_id_ = 0;
    return true;
  }

  const char *target_str = node->Attribute("param2");
  if(target_str == NULL) {
    this->target_type_ = SkillTargetType::TARGET;
    return true;
  }
  std::string target_type = target_str;
  if(target_type == "SELF") {
    this->target_type_ = SkillTargetType::SELF;
  } else if(target_type == "SELF_POSITION") {
    this->target_type_ = SkillTargetType::SELF_POSITION;
  } else if(target_type == "TARGET") {
    this->target_type_ = SkillTargetType::TARGET;
  } else if(target_type == "TARGET_POSITION") {
    this->target_type_ = SkillTargetType::TARGET_POSITION;
  } else {
    return false;
  }

  return true;
}

void SimpleUseSkillAction::OnEnter(BlackBoard *black_board) {}

RunningStatus::type SimpleUseSkillAction::OnExecute(BlackBoard *black_board) {
  if(black_board == NULL) {
    return RunningStatus::ERROR;
  }

  GameRole *me = black_board->GetGameRole();
  if(me == NULL) {
    return RunningStatus::ERROR;
  }

  // 检测是否可以使用技能
  if(black_board->CheckUseSkill(true) == false) {
    return RunningStatus::ERROR;
  }

  // 获取目标
  entity::EntityType::type target_type;
  core::uint64 target_id;
  black_board->GetTarget(target_type, target_id);

  // 目标不存在
  if(target_type == entity::EntityType::TYPE_MAX || target_id == 0) {
    return RunningStatus::ERROR;
  }

  // 发送使用技能请求
  request::RequestUseSkill request;
  request.__set_type_(black_board->GetType());
  request.__set_id_(black_board->GetID());
  request.__set_skill_id_(this->skill_id_);
  request.__set_target_type_(target_type);
  request.__set_target_id_(target_id);

  core::uint32 x = 0, y = 0;
  switch(this->target_type_) {
    case SkillTargetType::SELF:
      request.__set_target_type_(black_board->GetType());
      request.__set_target_id_(black_board->GetID());
      break;
    case SkillTargetType::SELF_POSITION:
      request.__set_x_(me->GetPosx());
      request.__set_y_(me->GetPosy());
      break;
    case SkillTargetType::TARGET:
      request.__set_target_type_(target_type);
      request.__set_target_id_(target_id);
      break;
    case SkillTargetType::TARGET_POSITION:
      black_board->GetTargetPosition(x, y);
      request.__set_x_(x);
      request.__set_y_(y);
      break;
    default:
      break;
  }

  if(ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_USE_SKILL,
        &request, sizeof(request)) != 0 || request.result_ != request::SkillResultType::SUCCESS) {
    return RunningStatus::ERROR;
  }

  return RunningStatus::EXECUTING;
}

void SimpleUseSkillAction::OnQuit(BlackBoard *black_board) {}

}  // namespace ai

}  // namespace server

}  // namespace game

