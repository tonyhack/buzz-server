//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-08 18:13:46.
// File name: target_exist_condition.cc
//
// Description:
// Define class TargetExistCondition.
//

#include "game_server/server/extension/ai/target_exist_condition.h"

#include "core/base/types.h"
#include "entity/entity_types.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/extension/ai/black_board.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

const std::string TargetExistCondition::kTypeName_ = "target_exist";

TargetExistCondition::TargetExistCondition() {}
TargetExistCondition::~TargetExistCondition() {}

bool TargetExistCondition::LoadConfigure(TiXmlElement *node) {
  return true;
}

bool TargetExistCondition::DoCheckEvalute(BlackBoard *black_board) const {
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

  return GameActorManager::GetInstance()->GetActor(target_id) != NULL;
}

}  // namespace ai

}  // namespace server

}  // namespace game

