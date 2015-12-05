//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-10 17:35:59.
// File name: actor_jump_scene_event_handler.cc
//
// Description:
// Define class ActorJumpSceneEventHandler.
//

#include "session_server/server/extension/team/actor_jump_scene_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "database_server/client/database_client.h"
#include "global/common_functions.h"
#include "session_server/server/event/session_actor_event_types.h"
#include "session_server/server/event/session_event_types.h"
#include "session_server/server/extension_manager.h"
#include "session_server/server/session_actor.h"
#include "session_server/server/session_actor_manager.h"
#include "session_server/server/extension/team/team.h"
#include "session_server/server/extension/team/team_actor.h"
#include "session_server/server/extension/team/team_actor_manager.h"
#include "session_server/server/extension/team/team_actor_pool.h"
#include "session_server/server/extension/team/team_manager.h"

namespace session {

namespace server {

namespace team {

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

  // 获取 TeamActor 对象
  TeamActor *team_actor = TeamActorManager::GetInstance()->Get(event->actor_);
  if(team_actor == NULL) {
    LOG_ERROR("获取 TeamActor(%lu) 失败", event->actor_);
    return ;
  }

  // 玩家是否有组队
  if(team_actor->GetTeamID() == 0) {
    return ;
  }

  // 得到玩家组队对象
  Team *team = TeamManager::GetInstance()->Get(team_actor->GetTeamID());
  if(team == NULL) {
    LOG_ERROR("获取 Team(%lu) 失败", team_actor->GetTeamID());
    return ;
  }

  // 分配组队BUFF
  team->SynchronizeBuff();

  gateway::protocol::MessageTeamMemberSynchronize synchronize;
  synchronize.__set_actor_(global::ToString(event->actor_));
  synchronize.__set_map_(team_actor->GetMap());

  team->BroadcastMessage(synchronize,
      gateway::protocol::MessageType::MESSAGE_TEAM_MEMBER_SYNCHRONIZE);
}

}  // namespace team

}  // namespace server

}  // namespace session

