//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-08-28 14:14:34.
// File name: actor_login_event_handler.cc
//
// Description:
// Define class ActorLoginEventHandler.
//

#include "session_server/server/extension/team/actor_login_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "database_server/client/database_client.h"
#include "session_server/server/event/session_actor_event_types.h"
#include "session_server/server/event/session_event_types.h"
#include "session_server/server/extension_manager.h"
#include "session_server/server/session_actor.h"
#include "session_server/server/session_actor_manager.h"
#include "session_server/server/extension/team/team_actor.h"
#include "session_server/server/extension/team/team_actor_manager.h"
#include "session_server/server/extension/team/team_actor_pool.h"

namespace session {

namespace server {

namespace team {

ActorLoginEventHandler::ActorLoginEventHandler() {}
ActorLoginEventHandler::~ActorLoginEventHandler() {}

bool ActorLoginEventHandler::Initialize() {
  // Follow 所有玩家登陆事件
  this->connection_ =
    coresh::CommunicatorSingleton::GetInstance().Follow(event::EventType::EVENT_ACTOR_LOGIN,
        coresh::Communicator::GetChannelAll(), event::ChannelType::CHANNEL_ACTOR,
        boost::bind(&ActorLoginEventHandler::OnEvent, this, _1, _2, _3, _4));

  return true;
}

void ActorLoginEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->connection_);
}

void ActorLoginEventHandler::OnEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventActorLogin) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数 message, size, channel_type 可能错误");
    return ;
  }

  event::EventActorLogin *event = (event::EventActorLogin *)message;

  // 从玩家管理器中获取玩家对象
  SessionActor *actor = SessionActorManager::GetInstance()->GetActor(event->id_);
  if(actor == NULL) {
    LOG_ERROR("玩家(%lu) 组队扩展登陆失败，获取 SessionActor 失败", event->id_);
    return ;
  }

  // 组队玩家对象是否存在
  TeamActor *team_actor = TeamActorManager::GetInstance()->Get(event->id_);
  if(team_actor != NULL) {
    LOG_ERROR("玩家(%lu) TeamActor对象 已存在", event->id_);
    return ;
  }

  // 分配组队玩家对象
  team_actor = TeamActorPool::GetInstance()->Allocate();
  if(team_actor == NULL) {
    LOG_ERROR("玩家(%lu) 分配 TeamActor对象 失败", event->id_);
    return ;
  }

  // 初始化组队玩家对象
  if(team_actor->Initialize(actor) == false) {
    TeamActorPool::GetInstance()->Deallocate(team_actor);
    LOG_ERROR("玩家(%lu) 初始化 TeamActor对象 失败", event->id_);
    return ;
  }

  // 加入到管理器
  if(TeamActorManager::GetInstance()->Add(team_actor) == false) {
    team_actor->Finalize();
    TeamActorPool::GetInstance()->Deallocate(team_actor);
    LOG_ERROR("玩家(%lu) TeamActor对象 加入管理器失败", event->id_);
    return ;
  }

  // 发送扩展加载成功事件
  event::EventActorExtensionLoadFinish load_finish;
  load_finish.__set_id_(event->id_);
  load_finish.__set_extension_type_(ExtensionType::TEAM);
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_ACTOR_EXTENSION_LOAD_FINISH, event->id_,
      event::ChannelType::CHANNEL_ACTOR, &load_finish,
      sizeof(event::EventActorExtensionLoadFinish));

  global::LogDebug("玩家(%lu, %s) 组队扩展加载成功",
      actor->GetActorID(), actor->GetName().c_str());
}

}  // namespace team

}  // namespace server

}  // namespace session

