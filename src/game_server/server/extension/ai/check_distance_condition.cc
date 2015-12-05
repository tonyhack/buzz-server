//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-16 17:32:22.
// File name: check_distance_condition.cc
//
// Description:
// Define class CheckDistanceCondition.
//

#include "game_server/server/extension/ai/check_distance_condition.h"

#include "core/base/types.h"
#include "entity/entity_types.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/extension/ai/black_board.h"
#include "global/distance_calculator.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

const std::string CheckDistanceCondition::kTypeName_ = "check_distance";

CheckDistanceCondition::CheckDistanceCondition() {}
CheckDistanceCondition::~CheckDistanceCondition() {}

bool CheckDistanceCondition::LoadConfigure(TiXmlElement *node) {
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

bool CheckDistanceCondition::DoCheckEvalute(BlackBoard *black_board) const {
  if(black_board == NULL) {
    return false;
  }

  // 获取目标
  entity::EntityType::type target_type = entity::EntityType::TYPE_MAX;
  core::uint64 target_id = 0;
  black_board->GetTarget(target_type, target_id);

  // 不是玩家或ID不合法
  if(target_type != entity::EntityType::TYPE_ACTOR ||
      target_id == 0) {
    return false;
  }

  GameActor *actor = GameActorManager::GetInstance()->GetActor(target_id);
  if(actor == NULL) {
    return false;
  }

  GameRole *me = black_board->GetGameRole();
  if(me == NULL) {
    return false;
  }

  core::uint32 src_x = 0, src_y = 0;
  core::uint32 dest_x = 0, dest_y = 0;

  me->GetPosition(src_x, src_y);
  actor->GetPosition(dest_x, dest_y);

  // 计算距离
  core::uint32 distance = global::DistanceCalculator::GetInstance()->Distance(
      src_x, src_y, dest_x, dest_y);
  if(distance == (core::uint32)-1) {
    return false;
  }

  //global::LogDebug("[Ai_Behaviour] CheckDistanceCondition::CheckEvaluate, blackboard(%d, %u)",
  //    black_board->GetType(), black_board->GetID());

  // 结果
  switch(this->operator_type_) {
    case RelationalOperator::GREATER:
      return distance > this->distance_;
    case RelationalOperator::LESS:
      return distance < this->distance_;
    case RelationalOperator::EQUAL:
      return distance == this->distance_;
    case RelationalOperator::NO_GREATER:
      return distance <= this->distance_;
    case RelationalOperator::NO_LESS:
      return distance >= this->distance_;
    case RelationalOperator::UNEQUAL:
      return distance != this->distance_;
    default:
      return false;
  }
}

}  // namespace ai

}  // namespace server

}  // namespace game

