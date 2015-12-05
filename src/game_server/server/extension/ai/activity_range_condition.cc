//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-08 14:40:03.
// File name: activity_range_condition.cc
//
// Description:
// Define class ActivityRangeCondition.
//

#include "game_server/server/extension/ai/activity_range_condition.h"

#include "core/base/types.h"
#include "entity/entity_types.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/game_npc.h"
#include "game_server/server/extension/ai/black_board.h"
#include "global/distance_calculator.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

const std::string ActivityRangeCondition::kTypeName_ = "check_activity_range";

ActivityRangeCondition::ActivityRangeCondition() {}

ActivityRangeCondition::~ActivityRangeCondition() {}

bool ActivityRangeCondition::LoadConfigure(TiXmlElement *node) {
  if(node == NULL) {
    return false;
  }
  const char *str = node->Attribute("param1");
  if(str == NULL) {
    global::LogError("%s:%d (%s) 找不到结点[node-param1]",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  // param1: 关系运行符
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
    global::LogError("%s:%d (%s) 参数 param1(%s) 不合法",
        __FILE__, __LINE__, __FUNCTION__, str);
    return false;
  }
  // param2: 距离值
  if(node->Attribute("param2", (int *)(&this->distance_)) == NULL) {
    global::LogError("%s:%d (%s) 找不到结点[node-param2]",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  return true;
}

bool ActivityRangeCondition::DoCheckEvalute(BlackBoard *black_board) const {
  if(black_board == NULL) {
    return false;
  }

  GameRole *role = black_board->GetGameRole();
  if(role == NULL) {
    global::LogError("%s:%d (%s) 获取 GameRole 失败");
    return false;
  }

  if(role->GetType() != entity::EntityType::TYPE_NPC) {
    global::LogError("%s:%d (%s) 角色类型错误");
    return false;
  }

  GameNpc *npc = (GameNpc *)role;

  core::uint32 born_x = 0, born_y = 0;
  core::uint32 x = 0, y = 0;

  npc->GetBornPosition(born_x, born_y);
  npc->GetPosition(x, y);

  // 得到距离
  core::uint32 distance = global::DistanceCalculator::GetInstance()->Distance(
      x, y, born_x, born_y);
  if(distance == (core::uint32)-1) {
    return false;
  }

  bool result = false;

  switch(this->operator_type_) {
    case RelationalOperator::GREATER:
      result = distance > this->distance_;
      break;
    case RelationalOperator::LESS:
      result = distance < this->distance_;
      break;
    case RelationalOperator::EQUAL:
      result = distance == this->distance_;
      break;
    case RelationalOperator::NO_GREATER:
      result = distance <= this->distance_;
      break;
    case RelationalOperator::NO_LESS:
      result = distance >= this->distance_;
      break;
    case RelationalOperator::UNEQUAL:
      result = distance != this->distance_;
      break;
    default:
      break;
  }

  return result;
}

}  // namespace ai

}  // namespace server

}  // namespace game

