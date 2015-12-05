//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-11 14:14:13.
// File name: actor_change_attribute_event_handler.cc
//
// Description:
// Define class ActorChangeAttributeEventHandler.
//

#include "game_server/server/extension/team/actor_change_attribute_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/session_channel.h"
#include "game_server/server/event/game_role_event_types.h"
#include "game_server/server/event/game_scene_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension/team/team_actor.h"
#include "game_server/server/extension/team/team_actor_manager.h"
#include "session_server/protocol/game_team_protocol_types.h"
#include "session_server/protocol/session_game_protocol_types.h"

namespace game {

namespace server {

namespace team {

ActorChangeAttributeEventHandler::ActorChangeAttributeEventHandler() {}
ActorChangeAttributeEventHandler::~ActorChangeAttributeEventHandler() {}

bool ActorChangeAttributeEventHandler::Initialize() {
  this->conn_change_hp_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ROLE_CHANGE_HP, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(
        &ActorChangeAttributeEventHandler::OnChangeHpEvent, this, _1, _2, _3, _4));

  this->conn_change_max_hp_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ROLE_CHANGE_MAX_HP, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(
        &ActorChangeAttributeEventHandler::OnChangeMaxHpEvent, this, _1, _2, _3, _4));

  this->conn_change_mp_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ROLE_CHANGE_MP, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(
        &ActorChangeAttributeEventHandler::OnChangeMpEvent, this, _1, _2, _3, _4));

  this->conn_change_max_mp_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ROLE_CHANGE_MAX_MP, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(
        &ActorChangeAttributeEventHandler::OnChangeMaxMpEvent, this, _1, _2, _3, _4));

  this->conn_change_location_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_SCENE_ROLE_CHANGE_LOCATION, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(
        &ActorChangeAttributeEventHandler::OnChangeLocationEvent, this, _1, _2, _3, _4));

  return true;
}

void ActorChangeAttributeEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_change_hp_);
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_change_max_hp_);
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_change_mp_);
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_change_max_mp_);
}

void ActorChangeAttributeEventHandler::OnChangeHpEvent(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventRoleChangeHp) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数错误");
    return ;
  }

  event::EventRoleChangeHp *event = (event::EventRoleChangeHp *)message;

  TeamActor *actor = TeamActorManager::GetInstance()->Get(event->id_);
  if(actor == NULL) {
    LOG_ERROR("获取 TeamActor(%lu) 失败", event->id_);
    return ;
  }

  if(actor->GetTeam() != 0) {
    session::protocol::TeamMemberAttrFields field;
    field.__set_type_(session::protocol::TeamMemberAttrType::HP);
    field.__set_value_(event->value_);
    session::protocol::MessageTeamMemberAttrSynchronize synchronize;
    synchronize.__set_actor_(event->id_);
    synchronize.attrs_.push_back(field);
    SessionChannel::SendMessage(synchronize,
        session::protocol::GameMessageType::MESSAGE_TEAM_MEMBER_ATTR_SYNCHRONIZE);
  }
}

void ActorChangeAttributeEventHandler::OnChangeMaxHpEvent(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventRoleChangeMaxHp) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数错误");
    return ;
  }

  event::EventRoleChangeMaxHp *event = (event::EventRoleChangeMaxHp *)message;

  TeamActor *actor = TeamActorManager::GetInstance()->Get(event->id_);
  if(actor == NULL) {
    LOG_ERROR("获取 TeamActor(%lu) 失败", event->id_);
    return ;
  }

  if(actor->GetTeam() != 0) {
    session::protocol::TeamMemberAttrFields field;
    field.__set_type_(session::protocol::TeamMemberAttrType::MAX_HP);
    field.__set_value_(event->value_);
    session::protocol::MessageTeamMemberAttrSynchronize synchronize;
    synchronize.__set_actor_(event->id_);
    synchronize.attrs_.push_back(field);
    SessionChannel::SendMessage(synchronize,
        session::protocol::GameMessageType::MESSAGE_TEAM_MEMBER_ATTR_SYNCHRONIZE);
  }
}

void ActorChangeAttributeEventHandler::OnChangeMpEvent(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventRoleChangeMp) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数错误");
    return ;
  }

  event::EventRoleChangeMp *event = (event::EventRoleChangeMp *)message;

  TeamActor *actor = TeamActorManager::GetInstance()->Get(event->id_);
  if(actor == NULL) {
    LOG_ERROR("获取 TeamActor(%lu) 失败", event->id_);
    return ;
  }

  if(actor->GetTeam() != 0) {
    session::protocol::TeamMemberAttrFields field;
    field.__set_type_(session::protocol::TeamMemberAttrType::MP);
    field.__set_value_(event->value_);
    session::protocol::MessageTeamMemberAttrSynchronize synchronize;
    synchronize.__set_actor_(event->id_);
    synchronize.attrs_.push_back(field);
    SessionChannel::SendMessage(synchronize,
        session::protocol::GameMessageType::MESSAGE_TEAM_MEMBER_ATTR_SYNCHRONIZE);
  }
}

void ActorChangeAttributeEventHandler::OnChangeMaxMpEvent(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventRoleChangeMaxMp) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数错误");
    return ;
  }

  event::EventRoleChangeMaxMp *event = (event::EventRoleChangeMaxMp *)message;

  TeamActor *actor = TeamActorManager::GetInstance()->Get(event->id_);
  if(actor == NULL) {
    LOG_ERROR("获取 TeamActor(%lu) 失败", event->id_);
    return ;
  }

  if(actor->GetTeam() != 0) {
    session::protocol::TeamMemberAttrFields field;
    field.__set_type_(session::protocol::TeamMemberAttrType::MAX_MP);
    field.__set_value_(event->value_);
    session::protocol::MessageTeamMemberAttrSynchronize synchronize;
    synchronize.__set_actor_(event->id_);
    synchronize.attrs_.push_back(field);
    SessionChannel::SendMessage(synchronize,
        session::protocol::GameMessageType::MESSAGE_TEAM_MEMBER_ATTR_SYNCHRONIZE);
  }
}

void ActorChangeAttributeEventHandler::OnChangeLocationEvent(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventSceneRoleChangeLocation) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数错误");
    return;
  }

  event::EventSceneRoleChangeLocation *event =
    (event::EventSceneRoleChangeLocation *)message;

  TeamActor *actor = TeamActorManager::GetInstance()->Get(event->id_);
  if(actor == NULL) {
    LOG_ERROR("获取 TeamActor(%lu) 失败", event->id_);
    return ;
  }

  if(actor->GetTeam() != 0) {
    session::protocol::MessageTeamMemberAttrSynchronize synchronize;
    synchronize.__set_actor_(event->id_);

    session::protocol::TeamMemberAttrFields field;
    field.__set_type_(session::protocol::TeamMemberAttrType::LOCATION_X);
    field.__set_value_(event->dest_x_);

    synchronize.attrs_.push_back(field);

    field.__set_type_(session::protocol::TeamMemberAttrType::LOCATION_Y);
    field.__set_value_(event->dest_y_);

    synchronize.attrs_.push_back(field);

    SessionChannel::SendMessage(synchronize,
        session::protocol::GameMessageType::MESSAGE_TEAM_MEMBER_ATTR_SYNCHRONIZE);
  }
}

}  // namespace team

}  // namespace server

}  // namespace game

