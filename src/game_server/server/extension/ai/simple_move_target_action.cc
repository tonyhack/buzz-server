//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-17 12:09:31.
// File name: simple_move_target_action.cc
//
// Description:
// Define class SimpleMoveTargetAction.
//

#include "game_server/server/extension/ai/simple_move_target_action.h"

#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/extension/ai/black_board.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

const std::string SimpleMoveTargetAction::kTypeName_ = "simple_move_target";

SimpleMoveTargetAction::SimpleMoveTargetAction() {}
SimpleMoveTargetAction::~SimpleMoveTargetAction() {}

bool SimpleMoveTargetAction::LoadConfigure(TiXmlElement *node) {
  return true;
}

void SimpleMoveTargetAction::OnEnter(BlackBoard *black_board) {}

RunningStatus::type SimpleMoveTargetAction::OnExecute(BlackBoard *black_board) {
  if(black_board == NULL) {
    return RunningStatus::ERROR;
  }

  if(black_board->CheckMove() == false) {
    return RunningStatus::EXECUTING;
  }

  // 获取目标
  entity::EntityType::type target_type = entity::EntityType::TYPE_MAX;
  core::uint64 target_id = 0;
  black_board->GetTarget(target_type, target_id);

  // 不是玩家或ID不合法
  if(target_type != entity::EntityType::TYPE_ACTOR ||
      target_id == 0) {
    return RunningStatus::ERROR;
  }

  GameActor *actor = GameActorManager::GetInstance()->GetActor(target_id);
  if(actor == NULL) {
    return RunningStatus::ERROR;
  }

  GameRole *me = black_board->GetGameRole();
  if(me == NULL) {
    return RunningStatus::ERROR;
  }

  core::uint32 src_x = 0, src_y = 0;
  core::uint32 dest_x = 0, dest_y = 0;

  me->GetPosition(src_x, src_y);
  actor->GetPosition(dest_x, dest_y);

  if(dest_x > src_x) {
    black_board->Move(entity::DirectionType::EAST);
  } else if(dest_x < src_x) {
    black_board->Move(entity::DirectionType::WEST);
  } else {
    if(dest_y > src_y) {
      black_board->Move(entity::DirectionType::SOUTH);
    } else if(dest_y < src_y) {
      black_board->Move(entity::DirectionType::NORTH);
    }
  }

  return RunningStatus::EXECUTING;
}

void SimpleMoveTargetAction::OnQuit(BlackBoard *black_board) {}

}  // namespace ai

}  // namespace server

}  // namespace game

