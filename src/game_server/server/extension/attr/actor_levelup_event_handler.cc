//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-08-08 15:15:12.
// File name: actor_levelup_event_handler.cc
//
// Description:
// Define class ActorLeveupEventHandler.
//

#include "game_server/server/extension/attr/actor_levelup_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/extension/attr/attr_actor.h"
#include "game_server/server/extension/attr/attr_actor_manager.h"
#include "game_server/server/extension/attr/attr_configure.h"
#include "game_server/server/extension/attr/configure.h"
#include "game_server/server/extension/attr/sword_actor.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace attr {

ActorLeveupEventHandler::ActorLeveupEventHandler() {}
ActorLeveupEventHandler::~ActorLeveupEventHandler() {}

bool ActorLeveupEventHandler::Initialize() {
  // EVENT_ACTOR_LEVEL_UP
  this->connection_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ACTOR_LEVEL_UP, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&ActorLeveupEventHandler::OnEvent,
        this, _1, _2, _3, _4));

  return true;
}

void ActorLeveupEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->connection_);
}

void ActorLeveupEventHandler::OnEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || size != sizeof(event::EventActorLevelUp)) {
    LOG_ERROR("Invalid params.");
    return ;
  }

  event::EventActorLevelUp *event = (event::EventActorLevelUp *)message;

  // 获取属性玩家对象
  AttrActor *attr_actor = AttrActorManager::GetInstance()->Get(event->actor_id_);
  if(attr_actor == NULL) {
    LOG_ERROR("Get AttrActor(%lu) failed.", event->actor_id_);
    return ;
  }

  // 获取游戏玩家对象
  GameActor *actor = attr_actor->GetActor();
  if(actor == NULL) {
    LOG_ERROR("Get GameActor(%lu) failed.", event->actor_id_);
    return ;
  }

  // 获取魔剑玩家对象
  SwordActor *sword_actor = attr_actor->GetSwordActor();
  if(sword_actor == NULL) {
    LOG_ERROR("Get SwordActor(%lu) failed.", event->actor_id_);
    return ;
  }

  const SwordCell *cell =
    Configure::GetInstance()->GetSword(sword_actor->GetCurrentStep());
  if(cell) {
    core::int32 level = actor->GetAttribute(entity::RoleAoiFields::LEVEL);
    sword_actor->SetLevel(level < cell->max_level_ ? level : cell->max_level_);
    // 更新战斗力
    sword_actor->UpdateSwordFightScore();
  }
}

}  // namespace attr

}  // namespace server

}  // namespace game

