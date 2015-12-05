//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-11 14:52:21.
// File name: game_message_handler.cc
//
// Description:
// Define class GameMessageHandler.
//

#include "session_server/server/extension/team/game_message_handler.h"

#include <boost/bind.hpp>

#include "global/common_functions.h"
#include "global/global_packet.h"
#include "session_server/protocol/game_team_protocol_types.h"
#include "session_server/protocol/session_game_protocol_types.h"
#include "session_server/server/extension_manager.h"
#include "session_server/server/extension/team/team.h"
#include "session_server/server/extension/team/team_actor.h"
#include "session_server/server/extension/team/team_actor_manager.h"
#include "session_server/server/extension/team/team_manager.h"
#include "session_server/server/extension/team/team_pool.h"

namespace session {

namespace server {

namespace team {

GameMessageHandler::GameMessageHandler() {}
GameMessageHandler::~GameMessageHandler() {}

bool GameMessageHandler::Initialize() {
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_TEAM_MEMBER_ATTR_SYNCHRONIZE,
      boost::bind(&GameMessageHandler::OnMessageTeamMemberAttrSynchronize,
        this, _1, _2, _3));

  return true;
}

void GameMessageHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_TEAM_MEMBER_ATTR_SYNCHRONIZE);
}

void GameMessageHandler::OnMessageTeamMemberAttrSynchronize(SessionTerminal *terminal,
    const char *data, size_t size) {
  if(data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return ;
  }

  session::protocol::MessageTeamMemberAttrSynchronize message;
  if(global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("消息解析 MessageTeamMemberAttrSynchronize 失败");
    return ;
  }

  TeamActor *actor = TeamActorManager::GetInstance()->Get(message.actor_);
  if(actor == NULL) {
    LOG_ERROR("获取 TeamActor(%lu) 失败", message.actor_);
    return ;
  }

  Team *team = TeamManager::GetInstance()->Get(actor->GetTeamID());
  if(team == NULL) {
    LOG_ERROR("获取 Team(%lu) 失败", actor->GetTeamID());
    return ;
  }

  gateway::protocol::MessageTeamMemberSynchronize synchronize;

  synchronize.__set_actor_(global::ToString(message.actor_));
  for(size_t pos = 0; pos < message.attrs_.size(); ++pos) {
    switch(message.attrs_[pos].type_) {
      case session::protocol::TeamMemberAttrType::HP:
        actor->SetCurrentHp(message.attrs_[pos].value_);
        synchronize.__set_current_hp_(message.attrs_[pos].value_);
        break;
      case session::protocol::TeamMemberAttrType::MAX_HP:
        actor->SetMaxHp(message.attrs_[pos].value_);
        synchronize.__set_max_hp_(message.attrs_[pos].value_);
        break;
      case session::protocol::TeamMemberAttrType::MP:
        actor->SetCurrentMp(message.attrs_[pos].value_);
        synchronize.__set_current_mp_(message.attrs_[pos].value_);
        break;
      case session::protocol::TeamMemberAttrType::MAX_MP:
        actor->SetMaxMp(message.attrs_[pos].value_);
        synchronize.__set_max_mp_(message.attrs_[pos].value_);
        break;
      case session::protocol::TeamMemberAttrType::LOCATION_X:
        actor->SetLocationX(message.attrs_[pos].value_);
        synchronize.__set_x_(message.attrs_[pos].value_);
        break;
      case session::protocol::TeamMemberAttrType::LOCATION_Y:
        actor->SetLocationY(message.attrs_[pos].value_);
        synchronize.__set_y_(message.attrs_[pos].value_);
        break;
      default:
        break;
    }
  }

  // 同步队友数据
  team->BroadcastMessage(synchronize,
      gateway::protocol::MessageType::MESSAGE_TEAM_MEMBER_SYNCHRONIZE);
}

}  // namespace team

}  // namespace server

}  // namespace session

