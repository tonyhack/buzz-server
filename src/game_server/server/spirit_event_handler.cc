#include "game_server/server/task_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "entity/constants_constants.h"
#include "game_server/server/configure.h"
#include "game_server/server/event/game_attr_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/load_functionality_configure.h"

namespace game {

namespace server {

SpiritEventHandler::SpiritEventHandler() {}
SpiritEventHandler::~SpiritEventHandler() {}

bool SpiritEventHandler::Initialize() {
  // EVENT_ATTR_SPIRIT_UPGRADE
  this->conn_level_upgrade_ =
    coresh::CommunicatorSingleton::GetInstance().Follow(event::EventType::EVENT_ATTR_SPIRIT_UPGRADE,
        coresh::Communicator::GetChannelAll(), event::ChannelType::CHANNEL_ACTOR,
        boost::bind(&SpiritEventHandler::OnLevelUpgrade, this, _1, _2, _3, _4));
  return true;
}

void SpiritEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_level_upgrade_);
}

void SpiritEventHandler::OnLevelUpgrade(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventAttrSpiritUpgrade) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数 message 或 size 错误");
    return ;
  }

  event::EventAttrSpiritUpgrade *event = (event::EventAttrSpiritUpgrade *)message;
  LoadFunctionalityConfigure::OpenCells& cells = Configure::GetInstance()->GetFunctionalityCell(
      entity::FunctionalityOpenType::SPIRIT_LEVEL);

  // 得到玩家对象
  GameActor *game_actor = GameActorManager::GetInstance()->GetActor(event->actor_id_);
  if(game_actor == NULL) {
    LOG_ERROR("玩家对象没有找到[%lu]", event->actor_id_);
    return ;
  }

  // 开启功能
  for(core::uint32 i = 0; i < cells.size(); ++i) {
    if(cells[i].param_ <= event->new_level_ &&
        game_actor->CheckFunctionalityState(cells[i].function_) == false) {
      game_actor->SetFunctionalityState(cells[i].function_, true, true);
      LOG_DEBUG("开启功能类型[%d]", cells[i].function_);
    }
  }
}

} // namespace server

} // namepsace game
