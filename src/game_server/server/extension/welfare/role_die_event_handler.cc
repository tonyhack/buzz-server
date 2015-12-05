
#include "game_server/server/extension/welfare/role_die_event_handler.h"

#include <boost/bind.hpp>
#include "game_server/server/event/game_role_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension/welfare/welfare_actor.h"
#include "game_server/server/extension/welfare/welfare_actor_manager.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace welfare {

RoleDieEventHandler::RoleDieEventHandler() {}

RoleDieEventHandler::~RoleDieEventHandler() {}

bool RoleDieEventHandler::Initialize() {
  this->conn_actor_killed_ = 
      coresh::CommunicatorSingleton::GetInstance().Follow(event::EventType::EVENT_ROLE_KILLED,
      coresh::Communicator::GetChannelAll(), event::ChannelType::CHANNEL_ACTOR,
      boost::bind(&RoleDieEventHandler::OnEventActor, this, _1, _2, _3, _4));

  this->conn_npc_killed_ = 
      coresh::CommunicatorSingleton::GetInstance().Follow(event::EventType::EVENT_ROLE_KILLED,
      coresh::Communicator::GetChannelAll(), event::ChannelType::CHANNEL_NPC,
      boost::bind(&RoleDieEventHandler::OnEventNpc, this, _1, _2, _3, _4));

  return true;
}

void RoleDieEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_actor_killed_);
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_npc_killed_);
}

void RoleDieEventHandler::OnEventNpc(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventRoleKilled) != size ||
      channel_type != event::ChannelType::CHANNEL_NPC) {
    LOG_ERROR("参数 message 或 size 错误");
    return ;
  }

  event::EventRoleKilled *event = (event::EventRoleKilled *)message;

  if(event->type_ == entity::EntityType::TYPE_ACTOR) {
    // 获取玩家对象
    WelfareActor *actor = WelfareActorManager::GetInstance()->Get(event->id_);
    if(actor == NULL) {
      LOG_ERROR("玩家(%lu)没有找到", event->id_);
      return ;
    }
    actor->GetAchieve()->OnEvent(event::EventType::EVENT_ROLE_KILLED, message, size);
  } 
  if(event->attacker_type_ == entity::EntityType::TYPE_ACTOR) {
    // 获取玩家对象
    WelfareActor *actor = WelfareActorManager::GetInstance()->Get(event->attacker_id_);
    if(actor == NULL) {
      LOG_ERROR("玩家(%lu)没有找到", event->attacker_id_);
      return ;
    }
    actor->GetAchieve()->OnEvent(event::EventType::EVENT_ROLE_KILLED, message, size);
  }
}

void RoleDieEventHandler::OnEventActor(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventRoleKilled) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数 message 或 size 错误");
    return ;
  }

  event::EventRoleKilled *event = (event::EventRoleKilled *)message;

  if(event->type_ == entity::EntityType::TYPE_ACTOR) {
    // 获取玩家对象
    WelfareActor *actor = WelfareActorManager::GetInstance()->Get(event->id_);
    if(actor == NULL) {
      LOG_ERROR("玩家(%lu)没有找到", event->id_);
      return ;
    }
    actor->GetAchieve()->OnEvent(event::EventType::EVENT_ROLE_KILLED, message, size);
  } 
  if(event->attacker_type_ == entity::EntityType::TYPE_ACTOR) {
    // 获取玩家对象
    WelfareActor *actor = WelfareActorManager::GetInstance()->Get(event->attacker_id_);
    if(actor == NULL) {
      LOG_ERROR("玩家(%lu)没有找到", event->attacker_id_);
      return ;
    }
    actor->GetAchieve()->OnEvent(event::EventType::EVENT_ROLE_KILLED, message, size);
  }

}

}  // namespace welfare

}  // namespace server

}  // namespace game
