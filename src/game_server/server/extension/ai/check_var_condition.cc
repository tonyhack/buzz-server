//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-06 15:18:18.
// File name: check_var_condition.cc
//
// Description:
// Define class CheckVarCondition.
//

#include "game_server/server/extension//ai/check_var_condition.h"

#include "core/base/types.h"
#include "game_server/server/extension/ai/black_board.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

const std::string CheckVarCondition::kTypeName_ = "check_var";

CheckVarCondition::CheckVarCondition() {}
CheckVarCondition::~CheckVarCondition() {}

bool CheckVarCondition::LoadConfigure(TiXmlElement *node) {
  if(node == NULL) {
    return false;
  }

  const char *str = node->Attribute("param1");
  if(str == NULL) {
    LOG_ERROR("找不到结点[node-param1]");
    return false;
  }
  this->var_ = str;

  str = node->Attribute("param2");
  if(str == NULL) {
    LOG_ERROR("找不到结点[node-param2]");
    return false;
  }
  if(strncmp(str, ">", sizeof(">")) == 0) {
    this->operator_type_ = RelationalOperator::GREATER;
  } else if(strncmp(str, "<", sizeof("<")) == 0) {
    this->operator_type_ = RelationalOperator::LESS;
  } else if(strncmp(str, "=", sizeof("=")) == 0) {
    this->operator_type_ = RelationalOperator::EQUAL;
  } else if(strncmp(str, "<=", sizeof("<=")) == 0) {
    this->operator_type_ = RelationalOperator::NO_GREATER;
  } else if(strncmp(str, ">=", sizeof(">=")) == 0) {
    this->operator_type_ = RelationalOperator::NO_LESS;
  } else if(strncmp(str, "!=", sizeof("!=")) == 0) {
    this->operator_type_ = RelationalOperator::UNEQUAL;
  } else {
    LOG_ERROR("参数 param2(%s) 不合法", str);
    return false;
  }

  if(node->Attribute("param3", (int *)(&this->value_)) == NULL) {
    global::LogError("%s:%d (%s) 找不到结点[node-param3]",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  return true;
}

bool CheckVarCondition::DoCheckEvalute(BlackBoard *black_board) const {
  if(black_board == NULL) {
    return false;
  }

  core::int64 value = black_board->GetVariables().Get(this->var_);
  if(value == core::kint64max) {
    return false;
  }

  switch(this->operator_type_) {
    case RelationalOperator::GREATER:
      return value > this->value_;
    case RelationalOperator::LESS:
      return value < this->value_;
    case RelationalOperator::EQUAL:
      return value == this->value_;
    case RelationalOperator::NO_GREATER:
      return value <= this->value_;
    case RelationalOperator::NO_LESS:
      return value >= this->value_;
    case RelationalOperator::UNEQUAL:
      return value != this->value_;
    default:
      return false;
  }
}

}  // namespace ai

}  // namespace server

}  // namespace game

