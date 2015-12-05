//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-20 17:27:53.
// File name: role_basic_status_change_handler.cc
//
// Description:
// Define class RoleBasicStatusChangeHandler.
//

#include "game_server/server/extension/ai/role_basic_status_change_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_role_event_types.h"
#include "game_server/server/extension/ai/ai_actor.h"
#include "game_server/server/extension/ai/ai_actor_manager.h"
#include "game_server/server/extension/ai/ai_controller.h"
#include "game_server/server/extension/ai/black_board.h"
#include "game_server/server/extension/ai/black_board_manager.h"
#include "global/channel.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

RoleBasicStatusChangeHandler::RoleBasicStatusChangeHandler() {}
RoleBasicStatusChangeHandler::~RoleBasicStatusChangeHandler() {}

bool RoleBasicStatusChangeHandler::Initialize() {
  // Follow 所有NPC状态转换事件
  this->conn_npc_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_BASIC_STATUS_CHANGE, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_NPC, boost::bind(&RoleBasicStatusChangeHandler::OnNpcEvent,
      this, _1, _2, _3, _4));

  // Follow 所有NPC状态转换事件
  this->conn_npc_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_BASIC_STATUS_CHANGE, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&RoleBasicStatusChangeHandler::OnActorEvent,
      this, _1, _2, _3, _4));

  return true;
}

void RoleBasicStatusChangeHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_npc_);
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_actor_);
}

void RoleBasicStatusChangeHandler::OnNpcEvent(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventBasicStatusChange) != size ||
      channel_type != event::ChannelType::CHANNEL_NPC) {
    global::LogError("%s:%d (%s) 参数 message 或 size 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  event::EventBasicStatusChange *event = (event::EventBasicStatusChange *)message;

  if(event->type_ != entity::EntityType::TYPE_NPC) {
    return ;
  }

  if(event->from_ == entity::RoleBasicStatus::STATUS_DIE ||
      event->to_ != entity::RoleBasicStatus::STATUS_DIE) {
    return ;
  }

  BlackBoard *blackboard = BlackBoardManager::GetInstance()->GetNpc(event->id_);
  if(blackboard == NULL) {
    global::LogError("%s:%d (%s) 获取 BlackBoard 对象失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 从AI控制器中移除
  if(blackboard->CheckHasAi()) {
    AiController::GetInstance()->RemoveBlackBoard(blackboard);
  }
}

void RoleBasicStatusChangeHandler::OnActorEvent(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventBasicStatusChange) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    global::LogError("%s:%d (%s) 参数 message 或 size 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  event::EventBasicStatusChange *event = (event::EventBasicStatusChange *)message;

  if(event->type_ != entity::EntityType::TYPE_ACTOR) {
    return ;
  }

  if(event->from_ == entity::RoleBasicStatus::STATUS_DIE ||
      event->to_ != entity::RoleBasicStatus::STATUS_DIE) {
    return ;
  }

  AiActor *actor = AiActorManager::GetInstance()->Get(event->id_);
  if(actor == NULL) {
    LOG_ERROR("获取 AiActor(%lu) 失败", event->id_);
    return ;
  }

  const AiActor::NpcSet &npcs = actor->GetNpcs();
  for(AiActor::NpcSet::const_iterator iterator = npcs.begin();
      iterator != npcs.end(); ++iterator) {
    BlackBoard *black_board = BlackBoardManager::GetInstance()->GetNpc(*iterator);
    if(black_board == NULL) {
      LOG_ERROR("获取 BlackBoard(%lu) 失败", *iterator);
      continue ;
    }
    black_board->GetThreat().Remove(event->id_);
  }
}

}  // namespace ai

}  // namespace server

}  // namespace game

