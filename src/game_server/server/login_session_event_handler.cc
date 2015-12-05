//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-30 11:08:26.
// File name: login_session_event_handler.cc
//
// Description:
// Define class LoginSessionEventHandler.
//

#include "game_server/server/login_session_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_system_event_types.h"
#include "game_server/server/session_channel.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "global/channel.h"
#include "global/game_misc.h"
#include "session_server/protocol/game_actor_protocol_types.h"

namespace game {

namespace server {

LoginSessionEventHandler::LoginSessionEventHandler() {}
LoginSessionEventHandler::~LoginSessionEventHandler() {}

bool LoginSessionEventHandler::Initialize() {
  // Follow 登陆 Session 成功事件
  this->connection_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_SYSTEM_LOGIN_SESSION, 0, event::ChannelType::CHANNEL_SYSTEM,
      boost::bind(&LoginSessionEventHandler::OnEvent, this, _1, _2, _3, _4));

  return true;
}

void LoginSessionEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->connection_);
}

void LoginSessionEventHandler::OnEvent(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  const GameActorManager::ActorHashmap &actors =
    GameActorManager::GetInstance()->GetActors();
  if(actors.empty() == true) {
    return ;
  }

  // 发送批量玩家信息到会话服务器
  session::protocol::MessageActorBatchLoginRequest request;
  session::protocol::SessionActorInfo info;

  GameActorManager::ActorHashmap::const_iterator iterator = actors.begin();
  for(; iterator != actors.end(); ++iterator) {
    if(iterator->second) {
      info.__set_id_(iterator->second->GetID());
      info.__set_name_(iterator->second->GetName());
      info.__set_level_(iterator->second->GetAttribute(entity::RoleAoiFields::LEVEL));
      info.__set_vocation_(
        (entity::VocationType::type)iterator->second->GetAttribute(entity::RoleAoiFields::VOCATION));
      info.__set_gender_(
        (entity::GenderType::type)iterator->second->GetAttribute(entity::ActorAoiFields::GENDER));
      info.__set_map_(global::GameMisc::GetSceneMapID(iterator->second->GetScene()));
      info.__set_gateway_(iterator->second->GetGateway());
      info.__set_functionality_state_(iterator->second->GetFunctionalityState());
      info.__set_axe_fight_score_(iterator->second->GetFightScore(entity::FightScoreType::AXE));
      info.__set_shoot_fight_score_(iterator->second->GetFightScore(entity::FightScoreType::SHOOT));
      info.__set_magic_fight_score_(iterator->second->GetFightScore(entity::FightScoreType::MAGIC));
      info.__set_last_logout_time_(iterator->second->GetLastLogoutTime());
      request.actors_.push_back(info);
    }
  }

  SessionChannel::SendMessage(request,
      session::protocol::GameMessageType::MESSAGE_ACTOR_BATCH_LOGIN_REQUEST);
}

}  // namespace server

}  // namespace game


