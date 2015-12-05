//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-23 11:55:13.
// File name: jump_scene_event_handler.cc
//
// Description:
// Define class JumpSceneEventHandler.
//

#include "game_server/server/extension/playing/jump_scene_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_playing_event_types.h"
#include "game_server/server/event/game_scene_event_types.h"
#include "game_server/server/extension/playing/playing.h"
#include "game_server/server/extension/playing/playing_actor.h"
#include "game_server/server/extension/playing/playing_actor_manager.h"
#include "game_server/server/extension/playing/playing_manager.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_playing_protocol_types.h"
#include "global/configure/configure.h"

namespace game {

namespace server {

namespace playing {

JumpSceneEventHandler::JumpSceneEventHandler() {}
JumpSceneEventHandler::~JumpSceneEventHandler() {}

bool JumpSceneEventHandler::Initialize() {
  this->connection_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_SCENE_JUMP_MAP, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&JumpSceneEventHandler::OnEvent,
        this, _1, _2, _3, _4));

  return true;
}

void JumpSceneEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->connection_);
}

void JumpSceneEventHandler::OnEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventSceneJumpMap) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数错误");
    return ;
  }

  event::EventSceneJumpMap *event = (event::EventSceneJumpMap *)message;

  PlayingActor *actor = PlayingActorManager::GetInstance()->Get(event->id_);
  if(actor == NULL) {
    LOG_ERROR("获取 PlayingActor(%lu) 失败", event->id_);
    return ;
  }

  // 玩家跳转场景时，从副本中删除
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

      gateway::protocol::MessagePlayingLeaveSynchronize synchronize;
      actor->SendMessage(synchronize,
          gateway::protocol::MessageType::MESSAGE_PLAYING_LEAVE_SYNCHRONIZE);

      if (playing_cell->camp_type_ != entity::PlayingCampType::ACTIVITY && playing->GetActorSize() == 0) {
        LOG_DEBUG("玩家跳转地图 销毁副本(%lu)", playing->GetID());
        // 离开副本就失败
        playing->RequireDestory();
      }
    }
    actor->SetPlaying(0);
  }

  GameActor *game_actor = actor->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("获取 GameActor 失败");
    return ;
  }

  playing_id = PlayingManager::GetInstance()->GetScenePlaying(game_actor->GetScene());
  if(playing_id == 0) {
    return ;
  }

  Playing *playing = PlayingManager::GetInstance()->Get(playing_id);
  if(playing == NULL) {
    LOG_ERROR("获取 Playing(%lu) 失败", playing_id);
    return ;
  }

  playing->AddActor(event->id_);

  actor->SetPlaying(playing_id);

  // 发送玩家进入副本事件
  event::EventPlayingActorEnter event_enter;
  event_enter.__set_actor_(event->id_);
  event_enter.__set_playing_(playing->GetID());
  event_enter.__set_template_(playing->GetTemplateID());
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_PLAYING_ACTOR_ENTER, playing->GetTemplateID(),
      event::ChannelType::CHANNEL_PLAYING, &event_enter,
      sizeof(event::EventPlayingActorEnter));
}

}  // namespace playing

}  // namespace server

}  // namespace game

