#include "game_server/server/extension/social/soul_upgrade_event_handler.h"

#include <boost/bind.hpp>

#include "global/logging.h"
#include "global/configure/configure.h"
#include "session_server/protocol/session_game_protocol_types.h"
#include "session_server/protocol/game_social_protocol_types.h"
#include "game_server/server/session_channel.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_soul_event_types.h"
#include "game_server/server/extension/social/social_actor.h"
#include "game_server/server/extension/social/social_actor_manager.h"

namespace game {

namespace server {

namespace social {

SoulUpgradeEventHandler::SoulUpgradeEventHandler() {}
SoulUpgradeEventHandler::~SoulUpgradeEventHandler() {}

bool SoulUpgradeEventHandler::Initialize() {
  // Follow 升级英灵事件
  this->conn_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_SOUL_UPGRADE_LEVEL, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR,
      boost::bind(&SoulUpgradeEventHandler::OnEvent, this, _1, _2, _3, _4));

  return true;
}

void SoulUpgradeEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_);
}

void SoulUpgradeEventHandler::OnEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if (NULL == message || size != sizeof(event::EventSoulUpgradeLevel) ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("Invalid params.");
    return;
  }

  event::EventSoulUpgradeLevel *event = (event::EventSoulUpgradeLevel *)message;

  if (SOCIAL_CONF()->GetHomelandSoul(event->soul_) == NULL) {
    return;
  }

  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(event->actor_);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", event->actor_);
    return;
  }

  session::protocol::MessageHomelandSoulSynchronize sync;
  sync.__set_actor_id_(event->actor_);

  session::protocol::HomelandSoulData data;
  data.__set_soul_id_(event->soul_);
  data.__set_soul_level_(event->level_);
  sync.souls_.push_back(data);
  SessionChannel::SendMessage(sync,
      session::protocol::GameMessageType::MESSAGE_HOMELAND_SOUL_SYNCHRONIZE);
}

}  // namespace social

}  // namespace server

}  // namespace game

