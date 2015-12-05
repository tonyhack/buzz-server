//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-05 16:51:55.
// File name: check_interval_seconds_condition.cc
//
// Description:
// Define class CheckIntervalSecondsCondition.
//

#include "game_server/server/extension/ai/check_interval_seconds_condition.h"

#include "core/base/types.h"
#include "entity/entity_types.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/global_time_tick.h"
#include "game_server/server/extension/ai/black_board.h"
#include "global/distance_calculator.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

const std::string CheckIntervalSecondsCondition::kTypeName_ = "check_interval_secs";

CheckIntervalSecondsCondition::CheckIntervalSecondsCondition() {}
CheckIntervalSecondsCondition::~CheckIntervalSecondsCondition() {}

bool CheckIntervalSecondsCondition::LoadConfigure(TiXmlElement *node) {
  if(node == NULL) {
    return false;
  }

  const char *var_str = node->Attribute("param1");
  if(var_str == NULL) {
    LOG_ERROR("找不到属性 [node-param1]");
    return false;
  }
  this->var_ = var_str;

  if(node->Attribute("param2", (int *)&this->interval_seconds_) == NULL) {
    LOG_ERROR("找不到属性 [node-param2]");
    return false;
  }

  return true;
}

bool CheckIntervalSecondsCondition::DoCheckEvalute(BlackBoard *black_board) const {
  if(black_board == NULL) {
    return false;
  }

  core::int64 value = black_board->GetVariables().Get(this->var_);
  if(value == core::kint64max) {
    return false;
  }

  time_t now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
  if((value + this->interval_seconds_) > now) {
    return false;
  }

  return true;
}

}  // namespace ai

}  // namespace server

}  // namespace game

