//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-05 11:41:45.
// File name: check_attack_distance_condition.cc
//
// Description:
// Define class CheckAttackDistanceCondition.
//

#include "game_server/server/extension//ai/check_attack_distance_condition.h"

#include "core/base/types.h"
#include "entity/entity_types.h"
#include "game_server/server/configure.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/game_npc.h"
#include "game_server/server/npc_configure.h"
#include "game_server/server/extension/ai/black_board.h"
#include "global/distance_calculator.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

const std::string CheckAttackDistanceCondition::kTypeName_ = "check_attack_distance";

CheckAttackDistanceCondition::CheckAttackDistanceCondition() {}
CheckAttackDistanceCondition::~CheckAttackDistanceCondition() {}

bool CheckAttackDistanceCondition::LoadConfigure(TiXmlElement *node) {
  if(node == NULL) {
    LOG_DEBUG("参数错误");
    return false;
  }

  if(node->Attribute("param1", (int *)&this->distance_) == NULL) {
    LOG_DEBUG("参数 找不到属性 param1");
    return false;
  }

  return true;
}

bool CheckAttackDistanceCondition::DoCheckEvalute(BlackBoard *black_board) const {
  if(black_board == NULL) {
    return false;
  }

  GameRole *me = black_board->GetGameRole();
  if(me == NULL || me->GetType() != entity::EntityType::TYPE_NPC) {
    return false;
  }

  GameNpc *npc = (GameNpc *)me;

  // 获取目标
  entity::EntityType::type target_type = entity::EntityType::TYPE_MAX;
  core::uint64 target_id = 0;
  black_board->GetTarget(target_type, target_id);

  if(target_type != entity::EntityType::TYPE_ACTOR ||
      target_id == 0) {
    return false;
  }

  GameActor *actor = GameActorManager::GetInstance()->GetActor(target_id);
  if(actor == NULL) {
    return false;
  }

  core::uint32 reference_distance = this->distance_;

  if(reference_distance == 0) {
    const NpcCell *cell =
      game::server::Configure::GetInstance()->GetNpcConfigure().GetNpc(npc->GetTemplateID());
    if(cell == NULL) {
      return false;
    }
    reference_distance = cell->attack_distance_;
  }

  core::uint32 src_x = 0, src_y = 0;
  core::uint32 dest_x = 0, dest_y = 0;

  npc->GetPosition(src_x, src_y);
  actor->GetPosition(dest_x, dest_y);

  // 计算距离
  core::uint32 distance = global::DistanceCalculator::GetInstance()->Distance(
      src_x, src_y, dest_x, dest_y);
  if(distance == (core::uint32)-1) {
    return false;
  }

  return distance < reference_distance;
}

}  // namespace ai

}  // namespace server

}  // namespace game

