//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-06 14:09:52.
// File name: check_attr_condition.cc
//
// Description:
// Define class CheckAttrCondition.
//

#include "game_server/server/extension/ai/check_attr_condition.h"

#include "core/base/types.h"
#include "entity/entity_types.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/extension/ai/black_board.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

const std::string CheckAttrCondition::kTypeName_ = "check_attr";

CheckAttrCondition::CheckAttrCondition() {}
CheckAttrCondition::~CheckAttrCondition() {}

bool CheckAttrCondition::LoadConfigure(TiXmlElement *node) {
  if(node == NULL) {
    return false;
  }

  const char *str = node->Attribute("param1");
  if(str == NULL) {
    LOG_ERROR("找不到属性[node->param1]");
    return false;
  }
  if(strncmp(str, "HP", sizeof("HP")) == 0) {
    this->condition_type_ = AttrConditionType::HP;
  } else if(strncmp(str, "HP_PERCENT", sizeof("HP_PERCENT")) == 0) {
    this->condition_type_ = AttrConditionType::HP_PERCENT;
  } else if(strncmp(str, "MP", sizeof("MP")) == 0) {
    this->condition_type_ = AttrConditionType::MP;
  } else if(strncmp(str, "MP_PERCENT", sizeof("MP_PERCENT")) == 0) {
    this->condition_type_ = AttrConditionType::MP_PERCENT;
  } else {
    LOG_ERROR("参数 param1(%s) 不合法", str);
    return false;
  }

  str = node->Attribute("param2");
  if(str == NULL) {
    LOG_ERROR("找不到属性[node->param2]");
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
    LOG_ERROR("参数 param1(%s) 不合法", str);
    return false;
  }

  if(node->Attribute("param3", (int *)&this->value_) == NULL) {
    LOG_ERROR("找不到属性[node->param3]");
    return false;
  }

  return true;
}

bool CheckAttrCondition::DoCheckEvalute(BlackBoard *black_board) const {
  if(black_board == NULL) {
    return false;
  }

  GameRole *me = black_board->GetGameRole();
  if(me == NULL) {
    return false;
  }

  core::int32 value = 0;
  core::int32 max_value = 0;
  bool percent = false;

  switch(this->condition_type_) {
    case AttrConditionType::HP:
      value = me->GetAttribute(entity::RoleAoiFields::CURRENT_HP);
      break;
    case AttrConditionType::HP_PERCENT:
      value = me->GetAttribute(entity::RoleAoiFields::CURRENT_HP);
      max_value = me->GetAttribute(entity::RoleAoiFields::MAX_HP);
      percent = true;
      break;
    case AttrConditionType::MP:
      value = me->GetAttribute(entity::RoleAoiFields::CURRENT_MP);
      break;
    case AttrConditionType::MP_PERCENT:
      value = me->GetAttribute(entity::RoleAoiFields::CURRENT_MP);
      max_value = me->GetAttribute(entity::RoleAoiFields::MAX_MP);
      percent = true;
      break;
    default:
      return false;
  }

  switch(this->operator_type_) {
    case RelationalOperator::GREATER:
      if(percent) {
        return value > (max_value * this->value_ / 100);
      } else {
        return value > this->value_;
      }
    case RelationalOperator::LESS:
      if(percent) {
        return value < (max_value * this->value_ / 100);
      } else {
        return value < this->value_;
      }
    case RelationalOperator::EQUAL:
      if(percent) {
        return value == (max_value * this->value_ / 100);
      } else {
        return value == this->value_;
      }
    case RelationalOperator::NO_GREATER:
      if(percent) {
        return value <= (max_value * this->value_ / 100);
      } else {
        return value <= this->value_;
      }
    case RelationalOperator::NO_LESS:
      if(percent) {
        return value >= (max_value * this->value_ / 100);
      } else {
        return value >= this->value_;
      }
    case RelationalOperator::UNEQUAL:
      if(percent) {
        return value != (max_value * this->value_ / 100);
      } else {
        return value != this->value_;
      }
    default:
      return false;
  }
}

}  // namespace ai

}  // namespace server

}  // namespace game
