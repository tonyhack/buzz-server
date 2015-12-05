//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-20 13:57:34.
// File name: item_container.cc
//
// Description:
// Define class ItemContainer.
//

#include "game_server/server/extension/item/item_container.h"

#include "database_server/client/database_client.h"
#include "database_server/protocol/storage_item_save_types.h"
#include "game_server/server/extension/item/configure.h"
#include "game_server/server/extension/item/equipment.h"
#include "game_server/server/extension/item/item.h"
#include "game_server/server/extension/item/item_actor.h"
#include "game_server/server/extension/item/item_builder.h"
#include "game_server/server/extension/item/item_configure.h"
#include "game_server/server/extension/item/material.h"
#include "game_server/server/extension/item/medicine.h"
#include "gateway_server/protocol/gateway_item_protocol_types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace item {

ItemContainer::ItemContainer()
  : capacity_(0), max_capacity_(0) {}

ItemContainer::~ItemContainer() {}

bool ItemContainer::Initialize(ItemActor *actor, core::uint16 capacity,
    core::uint16 max_capacity) {
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 参数 actor 不合法",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(max_capacity < capacity) {
    global::LogError("%s:%d (%s) 参数 capacity(%u), max_capacity(%u) 错误",
        __FILE__, __LINE__, __FUNCTION__, capacity, max_capacity);
    return false;
  }

  this->vocation_ = entity::VocationType::MAX;
  this->actor_ = actor;
  this->max_capacity_ = max_capacity;
  this->capacity_ = capacity;

  this->items_.resize(this->capacity_, NULL);

  return true;
}

void ItemContainer::Finalize() {
  this->actor_ = NULL;
  this->max_capacity_ = 0;
  this->capacity_ = 0;

  // 回收所有 Item
  ItemVector::iterator iterator = this->items_.begin();
  for(; iterator != this->items_.end(); ++iterator) {
    if(*iterator) {
      ItemBuilder::GetInstance()->Destory(*iterator);
    }
  }

  this->items_.clear();
}

bool ItemContainer::InitializeItems(const ItemFieldVector &items) {
  if(this->actor_ == NULL) {
    global::LogError("%s:%d (%s) 初始化容器道具失败，玩家指针为空",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  ItemFieldVector::const_iterator iterator = items.begin();
  for(; iterator != items.end(); ++iterator) {
    if(this->CheckLocationEmpty(iterator->location_) == false) {
      global::LogError("%s:%d (%s) 玩家(%lu) 容器(%u,%u,%u)，位置不合法",
          __FILE__, __LINE__, __FUNCTION__, this->actor_->GetID(),
          this->GetType(), iterator->location_, iterator->template_id_);
      return false;
    }
    Item *item = ItemBuilder::GetInstance()->Create(this->actor_, *iterator);
    if(item == NULL) {
      global::LogError("%s:%d (%s) 玩家(%lu) 容器(%u,%u,%u) 道具不存在",
          __FILE__, __LINE__, __FUNCTION__, this->actor_->GetID(),
          this->GetType(), iterator->location_, iterator->template_id_);
      return false;
    }
    // 如果是装备检测下是否过期
    if(item->GetItemType() == entity::ItemType::EQUIPMENT) {
      ((Equipment *)item)->ExpireCheck();
    }
    if(this->AddItem(item, iterator->location_, false, false) == false) {
      global::LogError("%s:%d (%s) 玩家(%lu) 容器(%u,%u,%u)，增加 Item 对象失败",
          __FILE__, __LINE__, __FUNCTION__, this->actor_->GetID(),
          this->GetType(), iterator->location_, iterator->template_id_);
      item->Finalize();
      ItemBuilder::GetInstance()->Destory(item);
      return false;
    }
    global::LogDebug("玩家(%lu) 容器(%u,%u) 增加道具(%u) 成功",
        this->actor_->GetID(), this->GetType(), iterator->location_,
        iterator->template_id_);
  }

  this->Synchronize();

  return true;
}

bool ItemContainer::ExportDatabase(database::protocol::StorageItemSaveRequest &data) {
  if(this->actor_ == NULL) {
    LOG_ERROR("actor_ 为NULL");
    return false;
  }

  core::uint16 location = 0;

  database::protocol::StorageItemField field;
  field.__set_vocation_(this->vocation_);
  ItemVector::iterator iterator = this->items_.begin();
  for(; iterator != this->items_.end(); ++iterator, ++location) {
    if(*iterator) {
      (*iterator)->ExportDatabaseField(field);
      field.__set_location_(location);
      data.fields_.push_back(field);
      data.__isset.fields_ = true;
    }
  }

  return true;
}

void ItemContainer::Save() {
  if(this->actor_ == NULL) {
    global::LogError("%s:%d (%s) 保存容器道具失败，玩家指针为空",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
 
  size_t number = 0;
  database::protocol::StorageItemSaveRequest request;
  request.__set_id_(this->actor_->GetID());
  request.__set_contain_type_(this->GetType());

  core::uint16 location = 0;

  database::protocol::StorageItemField field;
  ItemVector::iterator iterator = this->items_.begin();
  for(; iterator != this->items_.end(); ++iterator, ++location) {
    if(*iterator) {
      (*iterator)->ExportDatabaseField(field);
      field.__set_location_(location);
      request.fields_.push_back(field);
      request.__isset.fields_ = true;
    }
  }

  // 发送消息
  if(database::client::DatabaseClientSingleton::GetInstance().SendRequest(
        request, database::protocol::StorageType::STORAGE_ITEM_SAVE,
        this->actor_->GetID()) == false) {
    global::LogError("%s:%d (%s) 玩家(%lu) 保存容器(%u) 道具信息失败",
        __FILE__, __LINE__, __FUNCTION__, this->actor_->GetID(),
        this->GetType());
    return ;
  }

  global::LogDebug("玩家(%lu) 保存容器(%u) 道具信息",
      this->actor_->GetID(), this->GetType());
}

Item *ItemContainer::GetItemByTemplate(core::uint32 template_id) {
  Item *item = NULL;
  ItemVector::iterator iterator = this->items_.begin();
  for(; iterator != this->items_.end(); ++iterator) {
    item = *iterator;
    if(item && item->GetTemplateID() == template_id) {
      return item;
    }
  }
  return NULL;
}

Item *ItemContainer::GetItemByID(core::uint64 id) {
  Item *item = NULL;
  ItemVector::iterator iterator = this->items_.begin();
  for(; iterator != this->items_.end(); ++iterator) {
    item = *iterator;
    if(item && item->GetID() == id) {
      return item;
    }
  }
  return NULL;
}

bool ItemContainer::CheckAddItem(Item *item, core::uint16 index) const {
  if(item == NULL) {
    return false;
  }

  // 不能超过容量
  if(index >= this->GetCapacity()) {
    return false;
  }

  // 不能往有道具的位置增加
  if(this->items_[index] != NULL) {
    return false;
  }

  return true;
}

bool ItemContainer::AddItem(Item *item, core::uint16 index, bool sync, bool broadcast) {
  if(this->CheckAddItem(item, index) == false) {
    return false;
  }

  if(sync) {
    item->Synchronize();
  }

  return this->Assignment(item, index, sync);
}

core::int32 ItemContainer::GetItemNumByLocation(core::uint16 location) const {
  Item *item = this->GetItem(location);
  if(item == NULL) {
    return 0;
  }
  return item->GetCount();
}

bool ItemContainer::CheckAddItem(core::uint32 template_id,
    core::uint32 number, bool bind) const {
  const ItemCell *cell = Configure::GetInstance()->GetItem(template_id);
  if(cell == NULL) {
    return false;
  }
  core::uint32 location_num = 0;
  for(core::uint16 i = 0; i < this->GetCapacity(); ++i) {
    Item *item = this->GetItem(i);
    if(item == NULL) {
      location_num = cell->stack_count_;
    } else {
      if(item->GetTemplateID() == template_id &&
          item->GetBind() == bind) {
        location_num = cell->stack_count_ - item->GetCount();
      }
    }
    if(number > location_num) {
      number -= location_num;
    } else {
      return true;
    }
  }
  return false;
}

bool ItemContainer::CheckAddItems(std::vector<entity::AddItemFields>& checklists) const {
  core::int32 empty = 0;
  for(size_t j = 0; j < checklists.size(); ++j) {
    const ItemCell *cell = Configure::GetInstance()->GetItem(checklists[j].item_template_);
    if(cell == NULL) {
      return false;
    }
    core::uint32 number = checklists[j].number_;
    core::uint32 location_num = 0;
    for(core::uint16 i = 0; i < this->GetCapacity(); ++i) {
      Item *item = this->GetItem(i);
      if(item == NULL) {
        location_num = cell->stack_count_;
        empty++;
      } else {
        if(item->GetTemplateID() == (core::uint32)checklists[j].item_template_ &&
            item->GetBind() == checklists[j].bind_) {
          location_num = cell->stack_count_ - item->GetCount();
        }
      }
      if(number > location_num) {
        number -= location_num;
        continue;
      } else {
        number = 0;
        break;
      }
    }
    if(number != 0) {
      return false;
    }
  }
  if(empty == 0) {
    return true;
  }
  for(core::uint16 i = 0; i < this->GetCapacity(); ++i) {
    if(this->GetItem(i) == NULL) {
      --empty;
      if(empty == 0) {
        return true;
      }
    }
  }
  return false;
}

bool ItemContainer::AddItemsByLocation(core::uint16 location, core::uint32 template_id,
    core::uint32 number, bool simulate, bool bind, core::int32 intensify_level, const std::string *extra_info) {
  const ItemCell *cell = Configure::GetInstance()->GetItem(template_id);
  if(cell == NULL) {
    LOG_ERROR("配置文件没有找到[%d]",template_id);
    return false;
  }
  if(cell->stack_count_ < number || number <= 0) {
    LOG_ERROR("超出最大堆叠数目[%d]",number);
    return false;
  }
  Item *item = GetItem(location);
  if(item != NULL) {
    if(item->GetTemplateID() != template_id) {
      LOG_ERROR("添加道具[%d]失败[%d]", template_id, item->GetTemplateID());
      return false;
    }
    if(item->GetBind() != bind) {
      return false;
    }
    if(simulate == true) {
      return true;
    }
    if(item->GetCount() + number > cell->stack_count_) {
      LOG_ERROR("超过最大堆叠数");
      return false;
    }
  } else {
    if(simulate == true) {
      return true;
    }
    item = ItemBuilder::GetInstance()->Create(this->actor_, template_id);
    if(item == NULL) {
      LOG_ERROR("申请内存失败");
      return false;
    }
    if(bind == true) {
      item->SetAttribute(entity::ItemClientFields::BIND_TYPE, 1);
    } else {
      item->SetAttribute(entity::ItemClientFields::BIND_TYPE, 0);
    }
    item->SetAttribute(entity::ItemClientFields::STACK_COUNT, number);
    if(item->GetItemType() == entity::ItemType::EQUIPMENT) {
      Equipment *equip = (Equipment *)item;
      equip->SetAttribute(entity::EquipClientFields::INTENSIFY, intensify_level);
    }
    number = 0;
    if(this->AddItem(item, location) == false) {
      ItemBuilder::GetInstance()->Destory(item);
      return false;
    }
    return true;
  }
  item->SetAttribute(entity::ItemClientFields::STACK_COUNT,
      item->GetCount() + number, false);
  item->SetAttribute(entity::ItemClientFields::BIND_TYPE, bind == true ? 1 : 0, true);

  if(extra_info != NULL) {
    item->ImportExtraInfo(*extra_info);
  }

  return true;
}

bool ItemContainer::AddItems(core::uint32 template_id, core::uint32 number, bool bind,
    core::int32 intensify_level, const std::string *extra_info) {
  const ItemCell *cell = Configure::GetInstance()->GetItem(template_id);
  if(cell == NULL) {
    return false;
  }
  if(cell->stack_count_ != 1) {
    for(core::uint16 i = 0; i < this->capacity_; ++i) {
      Item *item = this->GetItem(i);
      if(item == NULL) {
        continue;
      }
      if(item->GetTemplateID() != template_id) {
        continue;
      }
      if(item->GetCount() >= (core::int32)cell->stack_count_) {
        continue;
      }
      if(item->GetBind()!= bind) {
        continue;
      }
      // 加入道具 
      if(cell->stack_count_ - item->GetCount() >= number) {
        item->SetAttribute(entity::ItemClientFields::STACK_COUNT,
            item->GetCount() + number);
        number = 0;
        return true;
      } else {
        number = number - (cell->stack_count_ - item->GetCount());
        item->SetAttribute(entity::ItemClientFields::STACK_COUNT,cell->stack_count_);
      }
    }
  }
  // 添加到空位
  while(number >= 1) {
    core::uint16 position = this->GetEmptyPosition();
    if(position == core::kuint16max) {
      return false;
    }
    Item *item = ItemBuilder::GetInstance()->Create(this->actor_, template_id);
    if(item == NULL) {
      return false;
    }
    if(bind == true) {
      item->SetAttribute(entity::ItemClientFields::BIND_TYPE, 1);
    } else {
      item->SetAttribute(entity::ItemClientFields::BIND_TYPE, 0);
    }
    if(number <= cell->stack_count_) {
      item->SetAttribute(entity::ItemClientFields::STACK_COUNT, number);
      number = 0;
    } else {
      item->SetAttribute(entity::ItemClientFields::STACK_COUNT, cell->stack_count_);
      number -= cell->stack_count_;
    }
    if(extra_info != NULL) {
      item->ImportExtraInfo(*extra_info);
    }
    if(item->GetItemType() == entity::ItemType::EQUIPMENT) {
      Equipment *equip = (Equipment *)item;
      equip->SetAttribute(entity::EquipClientFields::INTENSIFY, intensify_level);
    }
    if(this->AddItem(item, position) == false) {
      ItemBuilder::GetInstance()->Destory(item);
      return false;
    }
  }
  return true;
}

bool ItemContainer::CheckRemoveItem(Item *item) const {
  if(item == NULL) {
    return false;
  }

  // 下标不能超过容量
  if(item->GetLocation() >= this->GetCapacity()) {
    return false;
  }

  // 是否和当前道具相同
  if(item != this->items_[item->GetLocation()]) {
    return false;
  }

  return true;
}

bool ItemContainer::RemoveItem(Item *item, bool sync, bool broadcast) {
  if(this->actor_ == NULL || this->CheckRemoveItem(item) == false) {
    return false;
  }

  this->items_[item->GetLocation()] = NULL;

  if(sync) {
    gateway::protocol::MessageItemContainerOperate message;
    message.__set_container_type_(this->GetType());
    message.__set_location_(item->GetLocation());
    message.__set_operate_(gateway::protocol::ContainerOperateType::REMOVE);
    message.__set_item_id_(item->GetStringID());
    if(this->GetType() == entity::ItemContainerType::EQUIP) {
      message.__set_vocation_(this->vocation_);
    }

    this->actor_->SendMessage(message,
        gateway::protocol::MessageType::MESSAGE_ITEM_CONTAINER_OPERATE);
  }

  item->SetContainerLocation(entity::ItemContainerType::MAX, core::kuint16max);

  return true;
}

void ItemContainer::Arrange() {
  if(this->actor_ == NULL) {
    LOG_ERROR("整理容器时，玩家对象为空");
    return ;
  }

  this->Merge();
  std::sort(this->items_.begin(), this->items_.end(), ItemContainer::ArrageSorter);

  core::uint16 index = 0;
  for(; index < this->GetCapacity(); ++index) {
    if(this->items_[index] == NULL) {
      break;
    }
    this->items_[index]->SetLocation(index);
  }

  this->Synchronize();
}

core::uint16 ItemContainer::GetEmptyPosition() const {
  core::uint16 index = 0;
  for(; index < this->GetCapacity(); ++index) {
    if(this->items_[index] == NULL) {
      return index;
    }
  }

  return core::kuint16max;
}

core::uint16 ItemContainer::GetEmptyNumber() const {
  core::uint16 number = 0;
  core::uint16 index = 0;
  for(; index < this->GetCapacity(); ++index) {
    if(this->items_[index] != NULL) {
      ++number;
    }
  }
  return this->capacity_ - number;
}

bool ItemContainer::Assignment(Item *item, core::uint16 index, bool sync) {
  if(this->actor_ == NULL || this->CheckAddItem(item, index) == false) {
    return false;
  }
  this->items_[index] = item;

  item->SetContainerLocation(this->GetType(), index);

  if(sync) {
    gateway::protocol::MessageItemContainerOperate message;
    message.__set_container_type_(this->GetType());
    message.__set_location_(index);
    message.__set_operate_(gateway::protocol::ContainerOperateType::ADD);
    message.__set_item_id_(item->GetStringID());
    if(this->GetType() == entity::ItemContainerType::EQUIP) {
      message.__set_vocation_(this->vocation_);
    }

    this->actor_->SendMessage(message,
        gateway::protocol::MessageType::MESSAGE_ITEM_CONTAINER_OPERATE);
  }

  return true;
}

void ItemContainer::Synchronize() {
  if(this->actor_ == NULL) {
    LOG_ERROR("actor_ 为 NULL");
    return ;
  }

  gateway::protocol::MessageItemContainerInitialize message;
  message.__set_container_type_(this->GetType());
  message.__set_vocation_(this->vocation_);

  entity::ItemFields item_fields;
  ItemVector::iterator iterator = this->items_.begin();
  for(; iterator != this->items_.end(); ++iterator) {
    Item *item = *iterator;
    if(item) {
      item->ExportInitialField(item_fields);
      message.items_.push_back(item_fields);
    }
  }

  this->actor_->SendMessage(message,
        gateway::protocol::MessageType::MESSAGE_ITEM_CONTAINER_INITIALIZE);
}

void ItemContainer::Merge() {
  ItemVector::iterator iterator1 = this->items_.begin();
  for(; iterator1 != this->items_.end(); ++iterator1) {
    Item *item1 = *iterator1;
    if(item1 == NULL) {
      continue;
    }
    const ItemCell *cell = Configure::GetInstance()->GetItem(item1->GetTemplateID());
    if(cell == NULL) {
      LOG_ERROR("获取 itemCell(%d) 失败", item1->GetTemplateID());
      continue;
    }
    core::int32 stack_count1 = item1->GetAttribute(entity::ItemClientFields::STACK_COUNT);
    if(stack_count1 >= (core::int32)cell->stack_count_) {
      continue;
    }
    ItemVector::iterator iterator2 = iterator1 + 1;
    for(; iterator2 != this->items_.end(); ++iterator2) {
      Item *item2 = *iterator2;
      if(item2 == NULL || item1->GetTemplateID() != item2->GetTemplateID() ||
          item1->GetAttribute(entity::ItemClientFields::BIND_TYPE) !=
          item2->GetAttribute(entity::ItemClientFields::BIND_TYPE)) {
        continue;
      }
      core::int32 stack_count2 = item2->GetAttribute(entity::ItemClientFields::STACK_COUNT);
      core::int32 add_count = cell->stack_count_ - stack_count1;
      if(add_count < stack_count2) {
        item1->SetAttribute(entity::ItemClientFields::STACK_COUNT, cell->stack_count_, false);
        item2->SetAttribute(entity::ItemClientFields::STACK_COUNT, stack_count2 - add_count, false);
        break;
      } else {
        if(this->RemoveItem(item2, false) == false) {
          LOG_ERROR("删除Item失败");
          break;
        }
        ItemBuilder::GetInstance()->Destory(item2);
        item1->SetAttribute(entity::ItemClientFields::STACK_COUNT, stack_count1 + stack_count2, false);
      }
      stack_count1 = item1->GetAttribute(entity::ItemClientFields::STACK_COUNT);
      if(stack_count1 >= (core::int32)cell->stack_count_) {
        break;
      }
    }
  }
}

bool ItemContainer::ArrageSorter(const Item *item1, const Item *item2) {
  /*
     物品的类型  装备，药品，卡牌，其他，任务      
     装备：品质＞部位＞等级      
     部位：武器＞防具＞首饰，根据字段排序      
     药品：药品类别＞等级      
     卡牌：ID      
     材料：ID      
     所有相同道具按照：非绑定＞绑定      
  */
  if(item1 == NULL) {
    return false;
  }
  if(item1 != NULL && item2 == NULL) {
    return true;
  }

  if(item1->GetTemplateID() == item2->GetTemplateID()) {
    return item1->GetAttribute(entity::ItemClientFields::BIND_TYPE) <
      item2->GetAttribute(entity::ItemClientFields::BIND_TYPE);
  } else {
    return item1->GetTemplateID() < item2->GetTemplateID();
  }
}

}  // namespace item

}  // namespace server

}  // namespace game

