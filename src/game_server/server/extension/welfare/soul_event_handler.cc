
#include "game_server/server/extension/welfare/soul_event_handler.h"

#include <boost/bind.hpp>
#include "game_server/server/event/game_soul_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace welfare {

SoulEventHandler::SoulEventHandler() {}

SoulEventHandler::~SoulEventHandler() {}

bool SoulEventHandler::Initialize() {
  this->conn_soul_set_ = 
      coresh::CommunicatorSingleton::GetInstance().Follow(event::EventType::EVENT_SOUL_SET,
      coresh::Communicator::GetChannelAll(), event::ChannelType::CHANNEL_ACTOR,
      boost::bind(&SoulEventHandler::OnEventSoulSet, this, _1, _2, _3, _4));

  return true;
}

void SoulEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_soul_set_);
}

void SoulEventHandler::OnEventSoulSet(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventSoulSet) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数 message 或 size 错误");
    return ;
  }

  event::EventSoulSet *event = (event::EventSoulSet *)message;

}

}  // namespace welfare

}  // namespace server

}  // namespace game
