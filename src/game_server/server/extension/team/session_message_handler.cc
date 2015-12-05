//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-10 21:06:17.
// File name: session_message_handler.cc
//
// Description:
// Define SessionMessageHandler.
//

#include "game_server/server/extension/team/session_message_handler.h"

#include <boost/bind.hpp>

#include "game_server/server/extension_manager.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/session_channel.h"
#include "game_server/server/extension/team/team_actor.h"
#include "game_server/server/extension/team/team_actor_manager.h"
#include "game_server/server/request/game_buff_request_types.h"
#include "game_server/server/request/game_request_types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_team_protocol_types.h"
#include "global/common_functions.h"
#include "global/logging.h"
#include "session_server/protocol/game_team_protocol_types.h"
#include "session_server/protocol/session_game_protocol_types.h"

namespace game {

namespace server {

namespace team {

SessionMessageHandler::SessionMessageHandler() {}
SessionMessageHandler::~SessionMessageHandler() {}

bool SessionMessageHandler::Initialize() {
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_TEAM_SYNCHRONIZE, 
      boost::bind(&SessionMessageHandler::OnMessageTeamSynchronize, this, _1, _2));

  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_TEAM_BUFF_SYNCHRONIZE, 
      boost::bind(&SessionMessageHandler::OnMessageTeamBuffSynchronize, this, _1, _2));

  return true;
}

void SessionMessageHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_TEAM_SYNCHRONIZE);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_TEAM_BUFF_SYNCHRONIZE);
}

void SessionMessageHandler::OnMessageTeamSynchronize(const char *data,
    size_t size) {
  if(data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return ;
  }

  session::protocol::MessageTeamSynchronize message;
  if(global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("解析消息失败");
    return ;
  }

  TeamActor *actor = TeamActorManager::GetInstance()->Get(message.actor_);
  if(actor == NULL) {
    LOG_ERROR("获取 TeamActor(%lu) 失败", message.actor_);
    return ;
  }

  GameActor *game_actor = actor->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("获取 GameActor(%lu) 失败", message.actor_);
    return ;
  }

  // 之前没有组队，血量、魔法同步到会话
  if(actor->GetTeam() == 0) {
    session::protocol::MessageTeamMemberAttrSynchronize synchronize;
    synchronize.__set_actor_(message.actor_);
    session::protocol::TeamMemberAttrFields field;
    field.__set_type_(session::protocol::TeamMemberAttrType::HP);
    field.__set_value_(game_actor->GetAttribute(entity::RoleAoiFields::CURRENT_HP));
    synchronize.attrs_.push_back(field);
    field.__set_type_(session::protocol::TeamMemberAttrType::MAX_HP);
    field.__set_value_(game_actor->GetAttribute(entity::RoleAoiFields::MAX_HP));
    synchronize.attrs_.push_back(field);
    field.__set_type_(session::protocol::TeamMemberAttrType::MP);
    field.__set_value_(game_actor->GetAttribute(entity::RoleAoiFields::CURRENT_MP));
    synchronize.attrs_.push_back(field);
    field.__set_type_(session::protocol::TeamMemberAttrType::MAX_MP);
    field.__set_value_(game_actor->GetAttribute(entity::RoleAoiFields::MAX_MP));
    synchronize.attrs_.push_back(field);
    field.__set_type_(session::protocol::TeamMemberAttrType::LOCATION_X);
    field.__set_value_(game_actor->GetPosx());
    synchronize.attrs_.push_back(field);
    field.__set_type_(session::protocol::TeamMemberAttrType::LOCATION_Y);
    field.__set_value_(game_actor->GetPosy());
    synchronize.attrs_.push_back(field);
    SessionChannel::SendMessage(synchronize,
        session::protocol::GameMessageType::MESSAGE_TEAM_MEMBER_ATTR_SYNCHRONIZE);
  } else if(message.team_ == 0) {
    if(actor->GetTeamBuff() != 0) {
      // 清除原来的组队buff
      request::RequestBuffEnd request;
      request.__set_dest_entity_type_(entity::EntityType::TYPE_ACTOR);
      request.__set_dest_entity_id_(message.actor_);
      request.__set_buff_id_(actor->GetTeamBuff());
      if(ExtensionManager::GetInstance()->Request(
            request::RequestType::REQUEST_BUFF_END, &request,
            sizeof(request)) != 0 || request.succeed_ == false) {
        LOG_ERROR("移除组队BUFF(%u)失败", actor->GetTeamBuff());
        return ;
      }
    }

    actor->SetTeamBuff(0);
  }

  actor->SetTeam(message.team_);

  gateway::protocol::ActorTeamFields field;
  field.__set_actor_(global::ToString(message.actor_));
  field.__set_team_(global::ToString(message.team_));
  gateway::protocol::MessageTeamActorsBroadcast broadcast;
  broadcast.actors_.push_back(field);

  game_actor->SendMessageAround(broadcast,
      gateway::protocol::MessageType::MESSAGE_TEAM_ACTORS_BROADCAST);
}

void SessionMessageHandler::OnMessageTeamBuffSynchronize(const char *data,
    size_t size) {
  if(data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return ;
  }

  session::protocol::MessageTeamBuffSynchronize message;
  if(global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("解析消息失败");
    return ;
  }

  TeamActor *actor = TeamActorManager::GetInstance()->Get(message.actor_);
  if(actor == NULL) {
    LOG_ERROR("获取 TeamActor(%lu) 失败", message.actor_);
    return ;
  }

  if(actor->GetTeamBuff() > 0) {
    // 清除原来的组队buff
    request::RequestBuffEnd request;
    request.__set_dest_entity_type_(entity::EntityType::TYPE_ACTOR);
    request.__set_dest_entity_id_(message.actor_);
    request.__set_buff_id_(actor->GetTeamBuff());
    if(ExtensionManager::GetInstance()->Request(
          request::RequestType::REQUEST_BUFF_END, &request,
          sizeof(request)) != 0 || request.succeed_ == false) {
      LOG_ERROR("移除组队BUFF(%u)失败", actor->GetTeamBuff());
      return ;
    }
  }

  actor->SetTeamBuff(message.buff_);

  if(actor->GetTeamBuff() > 0) {
    // 加上当前的组队buff
    request::RequestBuffStart request;
    request.__set_src_entity_type_(entity::EntityType::TYPE_ACTOR);
    request.__set_src_entity_id_(message.actor_);
    request.__set_dest_entity_type_(entity::EntityType::TYPE_ACTOR);
    request.__set_dest_entity_id_(message.actor_);
    request.__set_buff_id_(actor->GetTeamBuff());
    if(ExtensionManager::GetInstance()->Request(
          request::RequestType::REQUEST_BUFF_START, &request,
          sizeof(request)) != 0 || request.succeed_ == false) {
      LOG_ERROR("增加组队BUFF(%u)失败", actor->GetTeamBuff());
      return ;
    }
  }
}

}  // namespace team

}  // namespace server

}  // namespace game

