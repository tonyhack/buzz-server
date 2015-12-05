//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-27 20:22:36.
// File name: move_target_action.cc
//
// Description:
// Define class MoveTargetAction.
//

#include "game_server/server/extension/ai/move_target_action.h"

#include "game_server/server/configure.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_npc.h"
#include "game_server/server/npc_configure.h"
#include "game_server/server/extension/ai/black_board.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

const std::string MoveTargetAction::kTypeName_ = "move_target";

MoveTargetAction::MoveTargetAction() {}
MoveTargetAction::~MoveTargetAction() {}

bool MoveTargetAction::LoadConfigure(TiXmlElement *node) {
  if(node == NULL) {
    global::LogError("%s:%d (%s) 加载AI结点失败，参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(node->Attribute("param1", (int *)(&this->max_path_)) == false) {
    global::LogError("%s:%d (%s) 找不到结点[node-param1]",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(node->Attribute("param2", (int *)(&this->move_target_radius_)) == false) {
    this->move_target_radius_ = -1;
  }

  return true;
}

void MoveTargetAction::OnEnter(BlackBoard *black_board) {
}

RunningStatus::type MoveTargetAction::OnExecute(BlackBoard *black_board) {
  if(black_board == NULL) {
    return RunningStatus::ERROR;
  }

  if(black_board->CheckMove() == false) {
    return RunningStatus::EXECUTING;
  }

  GameRole *me = black_board->GetGameRole();
  if(me == NULL || me->GetType() != entity::EntityType::TYPE_NPC) {
    return RunningStatus::ERROR;
  }

  GameNpc *npc = (GameNpc *)me;

  // 获取目标
  entity::EntityType::type target_type = entity::EntityType::TYPE_MAX;
  core::uint64 target_id = 0;
  black_board->GetTarget(target_type, target_id);

  // 不是玩家或ID不合法
  if(target_type != entity::EntityType::TYPE_ACTOR ||
      target_id == 0) {
    return RunningStatus::ERROR;
  }

  core::int32 reference_radius = this->move_target_radius_;

  if(reference_radius == -1) {
    const NpcCell *cell =
      game::server::Configure::GetInstance()->GetNpcConfigure().GetNpc(npc->GetTemplateID());
    if(cell == NULL) {
      return RunningStatus::ERROR;
    }
    reference_radius = cell->attack_distance_;
  }

  if(black_board->MoveTarget(reference_radius) == false) {
    return RunningStatus::ERROR;
  }

  return RunningStatus::EXECUTING;
}

void MoveTargetAction::OnQuit(BlackBoard *black_board) {
  if(black_board) {
    black_board->ClearPaths();
  }
}

}  // namespace ai

}  // namespace server

}  // namespace game

