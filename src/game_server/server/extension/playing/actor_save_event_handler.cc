//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-25 14:02:12.
// File name: actor_save_event_handler.cc
//
// Description:
// Define class ActorSaveEventHandler.
//

#include "game_server/server/extension/playing/actor_save_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension/playing/playing_actor.h"
#include "game_server/server/extension/playing/playing_actor_manager.h"
#include "global/global_packet.h"

namespace game {

namespace server {

namespace playing {

ActorSaveEventHandler::ActorSaveEventHandler() {}
ActorSaveEventHandler::~ActorSaveEventHandler() {}

bool ActorSaveEventHandler::Initialize() {
  this->connection_ =
    coresh::CommunicatorSingleton::GetInstance().Follow(event::EventType::EVENT_ACTOR_SAVE,
        coresh::Communicator::GetChannelAll(), event::ChannelType::CHANNEL_ACTOR,
        boost::bind(&ActorSaveEventHandler::OnEvent, this, _1, _2, _3, _4));

  return true;
}

void ActorSaveEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->connection_);
}

void ActorSaveEventHandler::OnEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventActorSave) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数 message 或 size 或 channel_type 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  event::EventActorSave *event = (event::EventActorSave *)message;

  LOG_DEBUG("Playing玩家对象保存");

  PlayingActor *actor = PlayingActorManager::GetInstance()->Get(event->id_);
  if(actor == NULL) {
    LOG_ERROR("获取 PlayingActor(%lu) 失败", event->id_);
    return ;
  }

  actor->Save();
}

}  // namespace playing

}  // namespace server

}  // namespace game

