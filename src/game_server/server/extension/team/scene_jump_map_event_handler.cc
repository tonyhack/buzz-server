//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-01-03 19:22:08.
// File name: scene_jump_map_event_handler.cc
//
// Description:
// Define class SceneJumpMapEventHandler.
//

#include "game_server/server/extension/team/scene_jump_map_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/session_channel.h"
#include "game_server/server/event/game_scene_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension/team/team_actor.h"
#include "game_server/server/extension/team/team_actor_manager.h"
#include "session_server/protocol/game_team_protocol_types.h"
#include "session_server/protocol/session_game_protocol_types.h"

namespace game {

namespace server {

namespace team {

SceneJumpMapEventHandler::SceneJumpMapEventHandler() {}
SceneJumpMapEventHandler::~SceneJumpMapEventHandler() {}

bool SceneJumpMapEventHandler::Initialize() {
  this->connection_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_SCENE_JUMP_MAP, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&SceneJumpMapEventHandler::OnEvent,
        this, _1, _2, _3, _4));

  return true;
}

void SceneJumpMapEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->connection_);
}

void SceneJumpMapEventHandler::OnEvent(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventSceneJumpMap) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数错误");
    return ;
  }

  event::EventSceneJumpMap *event = (event::EventSceneJumpMap *)message;

  TeamActor *actor = TeamActorManager::GetInstance()->Get(event->id_);
  if(actor == NULL) {
    LOG_ERROR("获取 TeamActor(%lu) 失败", event->id_);
    return ;
  }

  if(actor->GetTeam() != 0) {
    GameActor *game_actor = actor->GetActor();
    if(game_actor == NULL) {
      LOG_ERROR("获取 GameActor 失败");
      return;
    }

    session::protocol::MessageTeamMemberAttrSynchronize synchronize;
    synchronize.__set_actor_(event->id_);

    session::protocol::TeamMemberAttrFields field;
    field.__set_type_(session::protocol::TeamMemberAttrType::LOCATION_X);
    field.__set_value_(game_actor->GetPosx());

    synchronize.attrs_.push_back(field);

    field.__set_type_(session::protocol::TeamMemberAttrType::LOCATION_Y);
    field.__set_value_(game_actor->GetPosy());

    synchronize.attrs_.push_back(field);

    SessionChannel::SendMessage(synchronize,
        session::protocol::GameMessageType::MESSAGE_TEAM_MEMBER_ATTR_SYNCHRONIZE);
  }
}

}  // namespace team

}  // namespace server

}  // namespace game
