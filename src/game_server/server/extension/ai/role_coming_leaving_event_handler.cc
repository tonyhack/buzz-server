//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-13 14:25:48.
// File name: role_coming_leaving_event_handler.cc
//
// Description:
// Define class RoleComingLeavingEventHandler.
//

#include "game_server/server/extension/ai/role_coming_leaving_event_handler.h"

#include <boost/bind.hpp>

#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/game_npc.h"
#include "game_server/server/game_npc_manager.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_scene_event_types.h"
#include "game_server/server/extension/ai/black_board.h"
#include "game_server/server/extension/ai/black_board_manager.h"
#include "game_server/server/extension/ai/ai_actor.h"
#include "game_server/server/extension/ai/ai_actor_manager.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

RoleComingLeavingEventHandler::RoleComingLeavingEventHandler() {}
RoleComingLeavingEventHandler::~RoleComingLeavingEventHandler() {}

bool RoleComingLeavingEventHandler::Initialize() {
  // Follow 所有场景 role 靠近/远离 事件
  this->connection_coming_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_SCENE_ROLE_COMING, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_SCENE, boost::bind(
        &RoleComingLeavingEventHandler::OnRoleComingEvent, this, _1, _2, _3, _4));
  this->connection_leaving_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_SCENE_ROLE_LEAVING, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_SCENE, boost::bind(
        &RoleComingLeavingEventHandler::OnRoleLeavingEvent, this, _1, _2, _3, _4));

  return true;
}

void RoleComingLeavingEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->connection_coming_);
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->connection_leaving_);
}

void RoleComingLeavingEventHandler::OnRoleComingEvent(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventSceneRoleComing) != size ||
      channel_type != event::ChannelType::CHANNEL_SCENE) {
    global::LogError("%s:%d (%s) 参数 message 或 size 或 channel_type 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  event::EventSceneRoleComing *event = (event::EventSceneRoleComing *)message;

  if(event->type_ == entity::EntityType::TYPE_ACTOR) {
    std::vector<event::SceneRolePair>::const_iterator iterator = event->roles_.begin();
    for(; iterator != event->roles_.end(); ++iterator) {
      if(iterator->type_ == entity::EntityType::TYPE_NPC) {
        BlackBoard *black_board = BlackBoardManager::GetInstance()->GetNpc(iterator->id_);
        if(black_board) {
          black_board->AddEnemyCount();
        }
      }
    }
  } else if(event->type_ == entity::EntityType::TYPE_NPC) {
    BlackBoard *black_board = BlackBoardManager::GetInstance()->GetNpc(event->id_);
    if(black_board == NULL) {
      global::LogError("%s:%d (%s) 管理器中找不到 BlackBoard(npc, %u)",
          __FILE__, __LINE__, __FUNCTION__, event->id_);
      return ;
    }
    std::vector<event::SceneRolePair>::const_iterator iterator = event->roles_.begin();
    for(; iterator != event->roles_.end(); ++iterator) {
      if(iterator->type_ == entity::EntityType::TYPE_ACTOR) {
        black_board->AddEnemyCount();
      }
    }
  }
}

void RoleComingLeavingEventHandler::OnRoleLeavingEvent(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventSceneRoleLeaving) != size ||
      channel_type != event::ChannelType::CHANNEL_SCENE) {
    global::LogError("%s:%d (%s) 参数 message 或 size 或 channel_type 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  event::EventSceneRoleLeaving *event = (event::EventSceneRoleLeaving *)message;

  if(event->type_ == entity::EntityType::TYPE_ACTOR) {
    std::vector<event::SceneRolePair>::const_iterator iterator = event->roles_.begin();
    for(; iterator != event->roles_.end(); ++iterator) {
      if(iterator->type_ == entity::EntityType::TYPE_NPC) {
        GameNpc *game_npc = GameNpcManager::GetInstance()->Get(iterator->id_);
        if (game_npc == NULL) {
          continue;
        }
        BlackBoard *black_board = BlackBoardManager::GetInstance()->GetNpc(iterator->id_);
        if(black_board) {
          black_board->SubEnemyCount();
          if (game_npc->GetNpcType() != entity::NpcKind::TRANSPORT) {
            black_board->GetThreat().Remove(event->id_);
          }
          AiActor* ai_actor = AiActorManager::GetInstance()->Get(event->id_);
          if(ai_actor != NULL) {
            ai_actor->RemoveNpc(iterator->id_);
          } else {
            LOG_ERROR("玩家[%lu]离开时，删除怪物错误", event->id_);
          }
        }
      }
    }
  } else if(event->type_ == entity::EntityType::TYPE_NPC) {
    BlackBoard *black_board = BlackBoardManager::GetInstance()->GetNpc(event->id_);
    if(black_board == NULL) {
      global::LogError("%s:%d (%s) 管理器中找不到 BlackBoard(npc, %u)",
          __FILE__, __LINE__, __FUNCTION__, event->id_);
      return ;
    }
    std::vector<event::SceneRolePair>::const_iterator iterator = event->roles_.begin();
    for(; iterator != event->roles_.end(); ++iterator) {
      if(iterator->type_ == entity::EntityType::TYPE_ACTOR) {
        black_board->SubEnemyCount();
        black_board->GetThreat().Remove(event->id_);
        AiActor* ai_actor = AiActorManager::GetInstance()->Get(iterator->id_);
        if(ai_actor != NULL) {
          ai_actor->RemoveNpc(event->id_);
        } else {
          LOG_ERROR("玩家[%lu]离开时，删除怪物错误", iterator->id_);
        }
      }
    }
  }
}

}  // namespace ai

}  // namespace server

}  // namespace game

