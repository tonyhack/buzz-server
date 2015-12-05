//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-11-01 17:39:01.
// File name: change_vocation_event_handler.cc
//
// Description:
// Define class ChangeVocationEventHandler.
//

#include "game_server/server/extension/item/change_vocation_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "entity/functionality_types.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_role_event_types.h"
#include "game_server/server/extension/item/item_actor.h"
#include "game_server/server/extension/item/item_actor_manager.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace item {

ChangeVocationEventHandler::ChangeVocationEventHandler() {}
ChangeVocationEventHandler::~ChangeVocationEventHandler() {}

bool ChangeVocationEventHandler::Initialize() {
  this->conn_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ROLE_CHANGE_VOCATION, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&ChangeVocationEventHandler::OnEvent,
        this, _1, _2, _3, _4));

  return true;
}

void ChangeVocationEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_);
}

void ChangeVocationEventHandler::OnEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || size != sizeof(event::EventRoleChangeVocation) ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数错误");
    return ;
  }

  event::EventRoleChangeVocation *event = (event::EventRoleChangeVocation *)message;

  if(event->type_ != entity::EntityType::TYPE_ACTOR) {
    return ;
  }

  ItemActor *item_actor = ItemActorManager::GetInstance()->Get(event->id_);
  if(item_actor == NULL) {
    LOG_ERROR("获取 ItemActor(%lu) 失败", event->id_);
    return ;
  }

  GameActor *actor = item_actor->GetActor();
  if(actor == NULL) {
    LOG_ERROR("获取 GameActor(%lu) 失败", event->id_);
    return ;
  }

  EquipmentContainer *src_container = (EquipmentContainer *)item_actor->GetContainer(
      entity::ItemContainerType::EQUIP, event->src_vocation_);
  if(src_container == NULL) {
    LOG_ERROR("获取 ItemActor(%lu) GetContainer(%u,%u) 失败", event->id_,
        entity::ItemContainerType::EQUIP, event->src_vocation_);
    return ;
  }
  src_container->Sit();

  EquipmentContainer *dest_container = (EquipmentContainer *)item_actor->GetContainer(
      entity::ItemContainerType::EQUIP, (entity::VocationType::type)actor->GetAttribute(
        entity::RoleAoiFields::VOCATION));
  if(dest_container == NULL) {
    LOG_ERROR("获取 ItemActor(%lu) GetContainer(%u,%u) 失败", event->id_,
        entity::ItemContainerType::EQUIP, actor->GetAttribute(entity::RoleAoiFields::VOCATION));
    return ;
  }
  dest_container->Stand();
}

}  // namespace item

}  // namespace server

}  // namespace game

