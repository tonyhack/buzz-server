//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-11-27 20:49:54.
// File name: bar_finish_event_handler.cc
//
// Description:
//
#include "game_server/server/extension/attr/bar_finish_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "entity/constants_constants.h"
#include "game_server/server/configure.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_bar_event_types.h"
#include "game_server/server/extension/attr/attr_actor_manager.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/attr/attr_actor.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace attr {

BarFinishEventHandler::BarFinishEventHandler() {}
BarFinishEventHandler::~BarFinishEventHandler() {}

bool BarFinishEventHandler::Initialize() {
  // Follow 所有玩家登陆事件
  this->conn_spirit_transformed_ =
    coresh::CommunicatorSingleton::GetInstance().Follow(
        event::EventType::EVENT_BAR_FINISH,
        coresh::Communicator::GetChannelAll(), event::ChannelType::CHANNEL_ACTOR,
        boost::bind(&BarFinishEventHandler::OnEvent, this, _1, _2, _3, _4));

  return true;
}

void BarFinishEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_spirit_transformed_);
}

void BarFinishEventHandler::OnEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventBarFinish) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数 message 或 size 错误");
    return ;
  }

  event::EventBarFinish *event = (event::EventBarFinish *)message;

  if(event->type_ == entity::BarModuleType::BAR_TYPE_SPIRIT_TRANSFORMED) {
    OnTransformedEvent(channel, channel_type, message, size); 
  }
}

void BarFinishEventHandler::OnTransformedEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  event::EventBarFinish *event = (event::EventBarFinish *)message;

  // 得到玩家精灵对象
  AttrActor *attr_actor = AttrActorManager::GetInstance()->Get(event->actor_id_);
  if (NULL == attr_actor) {
    LOG_ERROR("Get AttrActor(%lu) from AttrActorManager failed.", event->actor_id_);
    return;
  }

  GameActor *game_actor = attr_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.", event->actor_id_);
    return;
  }

  SpiritActor *spirit_actor = attr_actor->GetSpiritActor();
  if(spirit_actor->TransformFacade(spirit_actor->GetFacadeID()) == false) {
    LOG_ERROR("精灵变身失败");
    this->NoticeSpiritTransFormed(game_actor, false);
    return ;
  }
  this->NoticeSpiritTransFormed(game_actor, true);
}

void BarFinishEventHandler::NoticeSpiritTransFormed(GameActor *actor, bool result) {
  gateway::protocol::MessageAttrSpiritTransformFacadeResponse response;
  response.__set_result_(result);
  actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_ATTR_SPIRIT_TRANSFORM_FACADE_RESPONSE);
}

}  // namespace attr

}  // namespace server

}  // namespace game
