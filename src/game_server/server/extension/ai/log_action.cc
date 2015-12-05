//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-12 14:54:26.
// File name: log_action.cc
//
// Description:
// Define class LogAction.
//

#include "game_server/server/extension/ai/log_action.h"

#include "game_server/server/extension/ai/black_board.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

const std::string LogAction::kTypeName_ = "log";

LogAction::LogAction() {}
LogAction::~LogAction() {}

bool LogAction::LoadConfigure(TiXmlElement *node) {
  if(node == NULL) {
    LOG_ERROR("参数错误");
    return false;
  }

  const char *log_str = node->Attribute("param1");
  if(log_str == NULL) {
    LOG_ERROR("找不到属性 [node-param1]");
    return false;
  }

  this->log_ = log_str;

  return true;
}

void LogAction::OnEnter(BlackBoard *black_board) {}

RunningStatus::type LogAction::OnExecute(BlackBoard *black_board) {
  if(black_board == NULL) {
    return RunningStatus::ERROR;
  }

  LOG_INFO("Role(%d,%lu):%s", black_board->GetType(), black_board->GetID(), this->log_.c_str());

  return RunningStatus::FINISH;
}

void LogAction::OnQuit(BlackBoard *black_board) {}

}  // namespace ai

}  // namespace server

}  // namespace game

