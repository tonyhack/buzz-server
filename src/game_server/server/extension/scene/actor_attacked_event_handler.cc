#include "game_server/server/extension/scene/actor_attacked_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "entity/constants_constants.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/event/game_skill_event_types.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace scene {

ActorAttackedEventHandler::ActorAttackedEventHandler() {}
ActorAttackedEventHandler::~ActorAttackedEventHandler() {}

bool ActorAttackedEventHandler::Initialize() {
  this->conn_hurt_ =
    coresh::CommunicatorSingleton::GetInstance().Follow(
        event::EventType::EVENT_SKILL_ATTACKED_SKILL,
        coresh::Communicator::GetChannelAll(), event::ChannelType::CHANNEL_ACTOR,
        boost::bind(&ActorAttackedEventHandler::OnEvent, this, _1, _2, _3, _4));

  return true;
}

void ActorAttackedEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_hurt_);
}

void ActorAttackedEventHandler::OnEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventAttackedSkill) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数 message 或 size 错误");
    return ;
  }

  event::EventAttackedSkill *event = (event::EventAttackedSkill *)message;

  if(event->type_ != entity::EntityType::TYPE_ACTOR) {
    LOG_ERROR("受到攻击的不是玩家");
    return ;
  }
  if(event->friend_type_ == 1) {
    return ;
  }
  GameActor *actor = GameActorManager::GetInstance()->GetActor(event->id_);
  if(actor == NULL) {
    LOG_ERROR("游戏玩家对象没有找到");
    return ;
  }
  if(actor->GetAttribute(entity::RoleAoiFields::STATUS) == entity::RoleBasicStatus::STATUS_ZAZEN) {
    actor->SetBasicStatus(entity::RoleBasicStatus::STATUS_COMMON);
  }
}

}  // namespace scene

}  // namespace server

}  // namespace game
