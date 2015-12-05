//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-05 17:23:31.
// File name: reset_interval_seconds_action.cc
//
// Description:
// Define class ResetIntervalSecondsAction.
//

#include "game_server/server/extension/ai/reset_interval_seconds_action.h"

#include "game_server/server/global_time_tick.h"
#include "game_server/server/extension/ai/black_board.h"
#include "global/logging.h"
#include "global/types.h"

namespace game {

namespace server {

namespace ai {

const std::string ResetIntervalSecondsAction::kTypeName_ = "reset_interval_secs";

ResetIntervalSecondsAction::ResetIntervalSecondsAction() {}
ResetIntervalSecondsAction::~ResetIntervalSecondsAction() {}

bool ResetIntervalSecondsAction::LoadConfigure(TiXmlElement *node) {
  if(node == NULL) {
    LOG_ERROR("参数错误");
    return false;
  }

  const char *var_str = node->Attribute("param1");
  if(var_str == NULL) {
    LOG_ERROR("找不到属性 [node-param1]");
    return false;
  }
  this->var_ = var_str;

  return true;
}

void ResetIntervalSecondsAction::OnEnter(BlackBoard *black_board) {}

RunningStatus::type ResetIntervalSecondsAction::OnExecute(BlackBoard *black_board) {
  if(black_board == NULL) {
    return RunningStatus::ERROR;
  }

  time_t now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();

  black_board->GetVariables().Set(this->var_, now);

  return RunningStatus::FINISH;
}

void ResetIntervalSecondsAction::OnQuit(BlackBoard *black_board) {}

}  // namespace ai

}  // namespace server

}  // namespace game

