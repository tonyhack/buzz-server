//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-05-30 01:05:55.
// File name: actor_jump_scene_event_handler.cc
//
// Description:
// Define class ActorJumpSceneEventHandler.
//

#include "session_server/server/extension/playing/actor_jump_scene_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "global/common_functions.h"
#include "session_server/server/event/session_actor_event_types.h"
#include "session_server/server/event/session_event_types.h"
#include "session_server/server/extension_manager.h"
#include "session_server/server/session_actor.h"
#include "session_server/server/session_actor_manager.h"
#include "session_server/server/extension/playing/playing.h"
#include "session_server/server/extension/playing/playing_manager.h"
#include "session_server/server/extension/playing/playing_actor.h"
#include "session_server/server/extension/playing/playing_actor_manager.h"

namespace session {

namespace server {

namespace playing {

ActorJumpSceneEventHandler::ActorJumpSceneEventHandler() {}
ActorJumpSceneEventHandler::~ActorJumpSceneEventHandler() {}

bool ActorJumpSceneEventHandler::Initialize() {
  this->connection_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ACTOR_JUMP_SCENE, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&ActorJumpSceneEventHandler::OnEvent,
        this, _1, _2, _3, _4));

  return true;
}

void ActorJumpSceneEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->connection_);
}

void ActorJumpSceneEventHandler::OnEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventActorJumpScene) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数 message, size, channel_type 可能错误");
    return ;
  }

  event::EventActorJumpScene *event = (event::EventActorJumpScene *)message;

  PlayingActor *actor = PlayingActorManager::GetInstance()->Get(event->actor_);
  if(actor == NULL) {
    LOG_ERROR("获取 PlayingActor(%lu) 失败", event->actor_);
    return;
  }

  SessionActor *session_actor = actor->GetActor();
  if (session_actor == NULL) {
    LOG_ERROR("获取 GameActor(%lu) 失败", event->actor_);
    return;
  }
  if (actor->GetPlaying() != 0) {
    Playing *playing = PlayingManager::GetInstance()->Get(actor->GetPlaying());
    if (playing == NULL) {
      LOG_ERROR("获取 Playing(%lu) 失败", actor->GetPlaying());
      return;
    }
    if (playing->GetScene() != session_actor->GetScene()) {
      actor->SetPlaying(0);
      PlayingManager::GetInstance()->RemoveActorPlaying(event->actor_);
    }
  }
}

}  // namespace playing

}  // namespace server

}  // namespace session

