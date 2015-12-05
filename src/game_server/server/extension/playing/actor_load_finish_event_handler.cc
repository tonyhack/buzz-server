//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-10-11 13:42:57.
// File name: actor_load_finish_event_handler.cc
//
// Description:
// Define class ActorLoadFinishEventHandler.
//

#include "game_server/server/extension/playing/actor_load_finish_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_playing_event_types.h"
#include "game_server/server/extension/playing/playing.h"
#include "game_server/server/extension/playing/playing_actor.h"
#include "game_server/server/extension/playing/playing_actor_manager.h"
#include "game_server/server/extension/playing/playing_manager.h"
#include "global/channel.h"
#include "global/game_misc.h"

namespace game {

namespace server {

namespace playing {

ActorLoadFinishEventHandler::ActorLoadFinishEventHandler() {}

ActorLoadFinishEventHandler::~ActorLoadFinishEventHandler() {}

bool ActorLoadFinishEventHandler::Initialize() {
  this->connection_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ACTOR_LOAD_FINISH, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&ActorLoadFinishEventHandler::OnEvent,
      this, _1, _2, _3, _4));

  return true;
}

void ActorLoadFinishEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->connection_);
}

void ActorLoadFinishEventHandler::OnEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventActorLoadFinish) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数错误");
    return ;
  }

  event::EventActorLoadFinish *event = (event::EventActorLoadFinish *)message;

  GameActor *game_actor = GameActorManager::GetInstance()->GetActor(event->id_);
  if(game_actor == NULL) {
    LOG_ERROR("获取 GameActor(%lu) 失败", event->id_);
    return ;
  }

  PlayingActor *playing_actor = PlayingActorManager::GetInstance()->Get(event->id_);
  if(playing_actor == NULL) {
    LOG_ERROR("获取 PlayingActor(%lu) 失败", event->id_);
    return ;
  }

  // 玩家是否在副本中
  core::uint64 playing_id =
    PlayingManager::GetInstance()->GetScenePlaying(game_actor->GetScene());
  if(playing_id != 0) {
    Playing *playing = PlayingManager::GetInstance()->Get(playing_id);
      if(playing == NULL) {
        LOG_ERROR("获取 Playing(%lu) 失败", playing_id);
        return ;
      }
    // 加入副本中
    playing->AddActor(game_actor->GetID());
    playing_actor->SetPlaying(playing_id);
    // 发送玩家进入副本事件
    event::EventPlayingActorEnter event_enter;
    event_enter.__set_actor_(game_actor->GetID());
    event_enter.__set_playing_(playing->GetID());
    event_enter.__set_template_(playing->GetTemplateID());
    coresh::CommunicatorSingleton::GetInstance().Broadcast(
        event::EventType::EVENT_PLAYING_ACTOR_ENTER, playing->GetTemplateID(),
        event::ChannelType::CHANNEL_PLAYING, &event_enter,
        sizeof(event::EventPlayingActorEnter));
  }
}

}  // namespace playing

}  // namespace server

}  // namespace game

