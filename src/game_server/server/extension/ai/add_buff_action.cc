//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-06 19:01:26.
// File name: add_buff_action.cc
//
// Description:
// Define class AddBuffAction.
//

#include "game_server/server/extension/ai/add_buff_action.h"

#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/ai/black_board.h"
#include "game_server/server/request/game_buff_request_types.h"
#include "global/logging.h" 

namespace game {

namespace server {

namespace ai {

const std::string AddBuffAction::kTypeName_ = "add_buff";

AddBuffAction::AddBuffAction() {}
AddBuffAction::~AddBuffAction() {}

bool AddBuffAction::LoadConfigure(TiXmlElement *node) {
  if(node == NULL) {
    LOG_ERROR("参数错误");
    return false;
  }

  if(node->Attribute("param1", (int *)&this->buff_id_) == NULL) {
    LOG_ERROR("找不到属性[node-param1]");
    return false;
  }

  this->target_type_ = TargetType::TARGET;

  const char *str = node->Attribute("param2");
  if(str != NULL) {
    std::string position_type = str;
    if(position_type == "SELF") {
      this->target_type_ = TargetType::SELF;
    } else if(position_type == "TARGET") {
      this->target_type_ = TargetType::TARGET;
    } else {
      LOG_ERROR("找不到属性[node-param2]");
      return false;
    }
  }

  return true;
}

void AddBuffAction::OnEnter(BlackBoard *black_board) {}

RunningStatus::type AddBuffAction::OnExecute(BlackBoard *black_board) {
  if(black_board == NULL) {
    return RunningStatus::ERROR;
  }

  request::RequestBuffStart request;
  request.__set_buff_id_(this->buff_id_);
  request.__set_src_entity_type_(black_board->GetType());
  request.__set_src_entity_id_(black_board->GetID());

  if(this->target_type_ == TargetType::SELF) {
    request.__set_dest_entity_type_(black_board->GetType());
    request.__set_dest_entity_id_(black_board->GetID());
  } else if(this->target_type_ == TargetType::TARGET) {
    entity::EntityType::type type = entity::EntityType::TYPE_MAX;
    core::uint64 id = 0;
    black_board->GetTarget(type, id);
    request.__set_dest_entity_type_(type);
    request.__set_dest_entity_id_(id);
  } else {
    return RunningStatus::ERROR;
  }

  if(ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_BUFF_START,
        &request, sizeof(request)) != 0 || request.succeed_ == false) {
    LOG_ERROR("请求 REQUEST_BUFF_START 失败");
    return RunningStatus::ERROR;
  }

  return RunningStatus::FINISH;
}

void AddBuffAction::OnQuit(BlackBoard *black_board) {}

}  // namespace ai

}  // namespace server

}  // namespace game

