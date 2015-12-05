
#include "game_server/server/extension/welfare/scene_event_handler.h"

#include <boost/bind.hpp>

#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/event/game_scene_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension/welfare/achieve_actor.h"
#include "game_server/server/extension/welfare/welfare_actor.h"
#include "game_server/server/extension/welfare/welfare_actor_manager.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_welfare_protocol_types.h"
#include "global/common_functions.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace welfare {

SceneEventHandler::SceneEventHandler() {}

SceneEventHandler::~SceneEventHandler() {}

bool SceneEventHandler::Initialize() {
  this->conn_broadcast_create_role_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_SCENE_BROADCAST_CREATE_ROLE, coresh::Communicator::GetChannelAll(), 
      event::ChannelType::CHANNEL_SCENE, boost::bind(
      &SceneEventHandler::OnEventBroadcastCreateRole, this, _1, _2, _3, _4));

  this->conn_create_aio_roles_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_SCENE_CREATE_AOI_ROLES, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_SCENE, boost::bind(
      &SceneEventHandler::OnEventCreateAoiRoles, this, _1, _2, _3, _4));

  return true;
}

void SceneEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_broadcast_create_role_);
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_create_aio_roles_);
}

void SceneEventHandler::OnEventBroadcastCreateRole(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventSceneBroadcastCreateRole) != size ||
      channel_type != event::ChannelType::CHANNEL_SCENE) {
    LOG_ERROR("参数 message 或 size 错误");
    return ;
  }

  event::EventSceneBroadcastCreateRole *event = (event::EventSceneBroadcastCreateRole *)message;

  if(event->type_ != entity::EntityType::TYPE_ACTOR) {
    return ;
  }

  // 获取玩家对象
  WelfareActor *actor = WelfareActorManager::GetInstance()->Get(event->id_);
  if(actor == NULL) {
    LOG_ERROR("玩家(%lu)没有找到", event->id_);
    return ;
  }
  AchieveActor *achieve_actor = actor->GetAchieve();

  // 广播周围玩家
  gateway::protocol::MessageWelfareTitleBroadCast msg;
  gateway::protocol::MessageTitleFiled field;
  field.__set_actor_id_(global::ToString(event->id_));
  field.__set_title_id_(achieve_actor->GetTitleID());
  msg.fields_.push_back(field);
  GameServerSingleton::GetInstance().MulticastMessage(msg,
      gateway::protocol::MessageType::MESSAGE_WELFARE_TITLE_BROADCAST,
      (core::uint64 *)&event->actors_[0], event->actors_.size());
}

void SceneEventHandler::OnEventCreateAoiRoles(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventSceneCreateAoiRoles) != size ||
      channel_type != event::ChannelType::CHANNEL_SCENE) {
    LOG_ERROR("参数 message 或 size 错误");
    return ;
  }

  event::EventSceneCreateAoiRoles *event = (event::EventSceneCreateAoiRoles *)message;

  // 获取玩家对象
  GameActor *channel_actor = GameActorManager::GetInstance()->GetActor(event->actor_);
  if(channel_actor == NULL) {
    LOG_ERROR("玩家对象没有找到(%lu)",event->actor_);
    return ;
  }

  size_t pos = 0;
  gateway::protocol::MessageWelfareTitleBroadCast msg;
  gateway::protocol::MessageTitleFiled field;
  for(; pos < event->roles_.size(); ++pos) {
    if(event->roles_[pos].type_ == entity::EntityType::TYPE_ACTOR) {
      GameActor *game_actor = GameActorManager::GetInstance()->GetActor(
          event->roles_[pos].id_);
      if(game_actor != NULL) {
        // 获取玩家对象
        WelfareActor *actor = WelfareActorManager::GetInstance()->Get(event->roles_[pos].id_);
        if(actor == NULL) {
          LOG_ERROR("玩家(%lu)没有找到", event->roles_[pos].id_);
          return ;
        }
        AchieveActor *achieve_actor = actor->GetAchieve();
        field.__set_actor_id_(global::ToString(event->roles_[pos].id_));
        field.__set_title_id_(achieve_actor->GetTitleID());
        msg.fields_.push_back(field);
      }
    }
    if(msg.fields_.size() >= kMaxCreateEquipSize_) {
      channel_actor->SendMessage(msg, gateway::protocol::MessageType::MESSAGE_WELFARE_TITLE_BROADCAST);
      msg.fields_.clear();
    }
  }

  if(msg.fields_.empty() == false) {
    channel_actor->SendMessage(msg, gateway::protocol::MessageType::MESSAGE_WELFARE_TITLE_BROADCAST);
  }
}

}  // namesapce welfare

}  // namespace server

}  // namespace game
