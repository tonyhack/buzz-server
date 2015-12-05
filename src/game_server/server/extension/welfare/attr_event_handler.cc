
#include "game_server/server/extension/welfare/attr_event_handler.h"

#include <boost/bind.hpp>
#include "game_server/server/event/game_attr_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension/welfare/achieve_actor.h"
#include "game_server/server/extension/welfare/welfare_actor.h"
#include "game_server/server/extension/welfare/welfare_actor_manager.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace welfare {

AttrEventHandler::AttrEventHandler() {}

AttrEventHandler::~AttrEventHandler() {}

bool AttrEventHandler::Initialize() {
  this->conn_spirit_upgrade_ = 
      coresh::CommunicatorSingleton::GetInstance().Follow(event::EventType::EVENT_ATTR_SPIRIT_UPGRADE,
      coresh::Communicator::GetChannelAll(), event::ChannelType::CHANNEL_ACTOR,
      boost::bind(&AttrEventHandler::OnEventSpiritUpgrade, this, _1, _2, _3, _4));

  return true;
}

void AttrEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_spirit_upgrade_);
}

void AttrEventHandler::OnEventSpiritUpgrade(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventAttrSpiritUpgrade) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数 message 或 size 错误");
    return ;
  }

  event::EventAttrSpiritUpgrade *event = (event::EventAttrSpiritUpgrade *)message;
  
  WelfareActor *actor = WelfareActorManager::GetInstance()->Get(event->actor_id_);
  if(actor == NULL) {
    LOG_ERROR("成就对象(%lu)没有找到", event->actor_id_);
    return ;
  }
  actor->GetAchieve()->OnEvent(event::EventType::EVENT_ATTR_SPIRIT_UPGRADE, event, size);
}

}  // namespace welfare

}  // namespace server 

}  // namespace game
