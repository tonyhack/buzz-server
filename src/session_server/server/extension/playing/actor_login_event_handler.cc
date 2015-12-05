//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-16 10:47:35.
// File name: actor_login_event_handler.cc
//
// Description:
// Define class ActorLoginEventHandler.
//

#include "session_server/server/extension/playing/actor_login_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "database_server/client/database_client.h"
#include "session_server/server/event/session_actor_event_types.h"
#include "session_server/server/event/session_event_types.h"
#include "session_server/server/extension_manager.h"
#include "session_server/server/session_actor.h"
#include "session_server/server/session_actor_manager.h"
#include "session_server/server/extension/playing/playing.h"
#include "session_server/server/extension/playing/playing_actor.h"
#include "session_server/server/extension/playing/playing_actor_manager.h"
#include "session_server/server/extension/playing/playing_actor_pool.h"
#include "session_server/server/extension/playing/playing_manager.h"

namespace session {

namespace server {

namespace playing {

ActorLoginEventHandler::ActorLoginEventHandler() {}
ActorLoginEventHandler::~ActorLoginEventHandler() {}

bool ActorLoginEventHandler::Initialize() {
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
    LOG_ERROR("参数错误");
    return ;
  }

  event::EventActorLogin *event = (event::EventActorLogin *)message;

  // 从玩家管理器中获取玩家对象
  SessionActor *actor = SessionActorManager::GetInstance()->GetActor(event->id_);
  if(actor == NULL) {
    LOG_ERROR("获取 SessionActor(%lu) 失败", event->id_);
    return ;
  }

  // PlayingActor 是否存在
  PlayingActor *playing_actor = PlayingActorManager::GetInstance()->Get(event->id_);
  if(playing_actor != NULL) {
    LOG_ERROR("玩家 PlayingActor(%lu) 已存在", event->id_);
    return ;
  }

  // 分配 PlayingActor 对象
  playing_actor = PlayingActorPool::GetInstance()->Allocate();
  if(playing_actor == NULL) {
    LOG_ERROR("分配 PlayingActor(%lu) 失败", event->id_);
    return ;
  }

  // 初始化 PlayingActor 对象
  if(playing_actor->Initialize(actor) == false) {
    PlayingActorPool::GetInstance()->Deallocate(playing_actor);
    LOG_ERROR("初始化 PlayingActor(%lu) 失败", event->id_);
    return ;
  }

  // 加入管理器
  if(PlayingActorManager::GetInstance()->Add(playing_actor) == false) {
    playing_actor->Finalize();
    PlayingActorPool::GetInstance()->Deallocate(playing_actor);
    LOG_ERROR("PlayingActor(%lu) 加入管理器失败", event->id_);
    return ;
  }

  core::uint64 playing_id = PlayingManager::GetInstance()->GetActorPlaying(event->id_);

  if(playing_id > 0) {
    Playing *playing = PlayingManager::GetInstance()->Get(playing_id);
    if(playing == NULL) {
      PlayingManager::GetInstance()->RemoveActorPlaying(event->id_);
    } else if(playing->GetStatus() != StatusType::END &&
        playing->GetStatus() != StatusType::DESTORY) {
      // 设置玩家可进入的副本
      playing_actor->SetPlaying(playing_id);
    }
  }

  // 发送扩展加载成功事件
  event::EventActorExtensionLoadFinish load_finish;
  load_finish.__set_id_(event->id_);
  load_finish.__set_extension_type_(ExtensionType::PLAYING);
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_ACTOR_EXTENSION_LOAD_FINISH, event->id_,
      event::ChannelType::CHANNEL_ACTOR, &load_finish,
      sizeof(event::EventActorExtensionLoadFinish));

  LOG_INFO("玩家(%lu, %s) Playing扩展加载成功", actor->GetActorID(),
      actor->GetName().c_str());
}

}  // namespace playing

}  // namespace server

}  // namespace session

