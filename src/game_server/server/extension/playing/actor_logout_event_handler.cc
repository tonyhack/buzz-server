//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-16 14:34:12.
// File name: actor_logout_event_handler.cc
//
// Description:
// Define class ActorLogoutEventHandler.
//

#include "game_server/server/extension/playing/actor_logout_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "database_server/client/database_client.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/event/game_playing_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension/playing/playing.h"
#include "game_server/server/extension/playing/playing_actor.h"
#include "game_server/server/extension/playing/playing_actor_manager.h"
#include "game_server/server/extension/playing/playing_actor_pool.h"
#include "game_server/server/extension/playing/playing_manager.h"
#include "global/configure/configure.h"

namespace game {

namespace server {

namespace playing {

ActorLogoutEventHandler::ActorLogoutEventHandler() {}
ActorLogoutEventHandler::~ActorLogoutEventHandler() {}

bool ActorLogoutEventHandler::Initialize() {
  // Follow 所有玩家登陆事件
  this->connection_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ACTOR_LOGOUT, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&ActorLogoutEventHandler::OnEvent,
        this, _1, _2, _3, _4));

  return true;
}

void ActorLogoutEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->connection_);
}

void ActorLogoutEventHandler::OnEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventActorLogout) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("%s:%d (%s) 参数 message 或 size 错误");
    return ;
  }

  event::EventActorLogout *event = (event::EventActorLogout *)message;

  PlayingActor *actor = PlayingActorManager::GetInstance()->Remove(event->id_);
  if(actor == NULL) {
    LOG_ERROR("移除 PlayingActor(%lu) 失败", event->id_);
    return ;
  }

  // 玩家下线时，从副本中删除
  core::uint64 playing_id = actor->GetPlaying();
  if(playing_id != 0) {
    Playing *playing = PlayingManager::GetInstance()->Get(playing_id);
    if(playing) {
      playing->RemoveActor(event->id_);

      // 发送玩家离开副本事件
      event::EventPlayingActorLeave event_leave;
      event_leave.__set_actor_(event->id_);
      event_leave.__set_playing_(playing->GetID());
      event_leave.__set_template_(playing->GetTemplateID());
      coresh::CommunicatorSingleton::GetInstance().Broadcast(
          event::EventType::EVENT_PLAYING_ACTOR_LEAVE, playing->GetTemplateID(),
          event::ChannelType::CHANNEL_PLAYING, &event_leave,
          sizeof(event::EventPlayingActorLeave));

      // 获取 PlayingCell
      const global::configure::PlayingCell *playing_cell =
        global::configure::ConfigureSingleton::GetInstance().GetPlayingCell(
            playing->GetTemplateID());
      if(playing_cell == NULL) {
        LOG_ERROR("获取 PlayingCell(%lu) 失败", playing->GetTemplateID());
        return ;
      }

      // 玩家离开副本就失败
      // playing->RequireDestory();
    }
    actor->SetPlaying(0);
  }

  // 回收对象
  actor->Finalize();
  PlayingActorPool::GetInstance()->Deallocate(actor);
}

}  // namespace playing

}  // namespace server

}  // namespace game

