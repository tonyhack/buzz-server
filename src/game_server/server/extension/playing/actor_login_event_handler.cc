//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-16 13:48:20.
// File name: actor_login_event_handler.cc
//
// Description:
// Define class ActorLoginEventHandler.
//

#include "game_server/server/extension/playing/actor_login_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "database_server/client/database_client.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension/playing/playing_actor.h"
#include "game_server/server/extension/playing/playing_actor_manager.h"

namespace game {

namespace server {

namespace playing {

ActorLoginEventHandler::ActorLoginEventHandler() {}
ActorLoginEventHandler::~ActorLoginEventHandler() {}

bool ActorLoginEventHandler::Initialize() {
  // Follow 所有玩家登陆事件
  this->connection_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ACTOR_LOGIN, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&ActorLoginEventHandler::OnEvent,
        this, _1, _2, _3, _4));

  return true;
}

void ActorLoginEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->connection_);
}

void ActorLoginEventHandler::OnEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventActorLogin) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数 message 或 size 或 channel_type 错误");
    return ;
  }

  event::EventActorLogin *event = (event::EventActorLogin *)message;

  // 从玩家管理器中获取玩家对象
  GameActor *actor = GameActorManager::GetInstance()->GetActor(event->id_);
  if(actor == NULL) {
    LOG_ERROR("玩家(%lu) Playing扩展登陆失败，获取 GameActor 失败", event->id_);
    return ;
  }

  PlayingActor *playing_actor = PlayingActorManager::GetInstance()->Get(event->id_);
  if(playing_actor != NULL) {
    LOG_ERROR("玩家(%lu) Playing扩展已存在", event->id_);
    return ;
  }

  database::protocol::StoragePlayingActorLoginRequest request;
  request.__set_actor_id_(event->id_);
  if(database::client::DatabaseClientSingleton::GetInstance().SendRequest(
        request, database::protocol::StorageType::STORAGE_PLAYING_ACTOR_LOGIN,
        event->id_) == false) {
    LOG_ERROR("向数据库代理服务器请求失败");
    return ;
  }
}

}  // namespace playing

}  // namespace server

}  // namespace game

