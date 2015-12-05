//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-21 14:42:37.
// File name: packet_container.cc
//
// Description:
// Define class PacketContainer.
//

#include "game_server/server/extension/item/packet_container.h"

#include "entity/actor_types.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/extension/item/item.h"
#include "game_server/server/extension/item/item_actor.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace item {

PacketContainer::PacketContainer() : load_finish_(false) {}
PacketContainer::~PacketContainer() {}

bool PacketContainer::Initialize(ItemActor *actor, core::uint16 capacity,
    core::uint16 max_capacity) {
  if(ItemContainer::Initialize(actor, capacity, max_capacity) == false) {
    global::LogError("%s:%d (%s) 初始化 ItemContainer 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->SetLoadFinish(false);

  return true;
}

void PacketContainer::Finalize() {
  ItemContainer::Finalize();
}

void PacketContainer::ExtendCapacity(core::int32 extend) {
  if(this->actor_ == NULL) {
    LOG_ERROR("actor_ 为 空");
    return ;
  }

  GameActor *actor = this->actor_->GetActor();
  if(actor == NULL) {
    LOG_ERROR("获取 GameActor 对象失败");
    return ;
  }

  actor->SetAttribute(entity::ActorClientFields::PACKET_CAPACITY,
      actor->GetAttribute(entity::ActorClientFields::PACKET_CAPACITY) + extend);

  this->capacity_ = actor->GetAttribute(entity::ActorClientFields::PACKET_CAPACITY);

  this->items_.resize(this->capacity_);
}

core::uint32 PacketContainer::GetItemNumber(core::uint32 template_id,
    entity::BindRequireType::type bind) {
  core::uint32 own_number = 0;
  ItemVector::iterator iterator = this->items_.begin();
  for(; iterator != this->items_.end(); ++iterator) {
    Item *item = *iterator;
    if(item == NULL || item->GetTemplateID() != template_id) {
      continue;
    }
    core::uint32 bind_type = item->GetAttribute(entity::ItemClientFields::BIND_TYPE);
    if(bind == entity::BindRequireType::NOT_BIND && bind_type != 0) {
      continue;
    }
    if(bind == entity::BindRequireType::BIND && bind_type == 0) {
      continue;
    }
    own_number += item->GetAttribute(entity::ItemClientFields::STACK_COUNT);
  }

  return own_number;
}

core::uint32 PacketContainer::RemoveItem(core::uint32 template_id, core::uint32 number,
    entity::BindRequireType::type &bind) {
  core::uint32 delete_number = number;
  core::uint16 location = 0;

  bool spend_bind = false;

  for(; location < this->items_.size(); ++location) {
    Item *item = this->GetItem(location);
    if(item == NULL || item->GetTemplateID() != template_id) {
      continue;
    }
    core::uint32 bind_type = item->GetAttribute(entity::ItemClientFields::BIND_TYPE);
    if(bind == entity::BindRequireType::NOT_BIND && bind_type != 0) {
      continue;
    }
    if(bind == entity::BindRequireType::BIND && bind_type == 0) {
      continue;
    }
    if(bind == entity::BindRequireType::BOTH && bind_type == 0) {
      continue;
    }
    core::uint32 stack_count = item->GetAttribute(entity::ItemClientFields::STACK_COUNT);
    if(stack_count > delete_number) {
      item->SetAttribute(entity::ItemClientFields::STACK_COUNT, stack_count - delete_number);
      delete_number = 0;
    } else {
      if(this->RemoveItem(item, true) == true) {
        delete_number -= stack_count;
      }
    }
    // 是否消耗了绑定道具
    if(bind_type != 0) {
      spend_bind = true;
    }
    if(delete_number <= 0) {
      break;
    }
  }

  // 选择了可删除道具为绑定或非绑定道具，则进行第二遍删除非绑道具
  if(delete_number > 0 && bind == entity::BindRequireType::BOTH) {
    for(location = 0; location < this->items_.size(); ++location) {
      Item *item = this->GetItem(location);
      if(item == NULL || item->GetTemplateID() != template_id) {
        continue;
      }
      core::uint32 stack_count = item->GetAttribute(entity::ItemClientFields::STACK_COUNT);
      if(stack_count > delete_number) {
        item->SetAttribute(entity::ItemClientFields::STACK_COUNT, stack_count - delete_number);
        delete_number = 0;
      } else {
        if(this->RemoveItem(item, true) == true) {
          delete_number -= stack_count;
        }
      }
      if(delete_number <= 0) {
        break;
      }
    }
  }

  if(spend_bind == true) {
    bind = entity::BindRequireType::BIND;
  }

  return number - delete_number;
}

}  // namespace item

}  // namespace server

}  // namespace game

