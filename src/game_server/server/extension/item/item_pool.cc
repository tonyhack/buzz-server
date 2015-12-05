//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-20 15:34:22.
// File name: item_pool.cc
//
// Description:
// Define class ItemPool.
//

#include "game_server/server/extension/item/item_pool.h"

#include "game_server/server/extension/item/equipment.h"
#include "game_server/server/extension/item/item.h"
#include "game_server/server/extension/item/material.h"
#include "game_server/server/extension/item/medicine.h"

namespace game {

namespace server {

namespace item {

bool ItemPool::Initialize(size_t initial_number, size_t extend_number) {
  global::LogDebug("ItemPool::Initialize initial_number(%lu) extend_number(%lu)",
      initial_number, extend_number);
  if(this->equipments_.Initialize(initial_number, extend_number) &&
      this->medicines_.Initialize(initial_number, extend_number) &&
      this->materials_.Initialize(initial_number, extend_number) /* &&
      this->cards_.Initialize(initial_number, extend_number) &&
      this->tasks_.Initialize(initial_number, extend_number)*/) {
    this->sequence_ = 0;
  global::LogDebug("ItemPool::Initialize true");
    return true;
  }
  global::LogDebug("ItemPool::Initialize false");
  return false;
}

void ItemPool::Finalize() {
  this->equipments_.Finalize();
  this->materials_.Finalize();
  this->medicines_.Finalize();
  //this->cards_.Finalize();
  //this->materials_.Finalize();
  //this->tasks_.Finalize();
}

Item *ItemPool::Allocate(entity::ItemType::type type) {
  Item *item = NULL;
  switch(type) {
    case entity::ItemType::EQUIPMENT:
      item = this->equipments_.Allocate();
      if(item) { ((Equipment *)item)->Allocate(this->sequence_++); }
      break;
    case entity::ItemType::MEDICINE:
      item = this->medicines_.Allocate();
      if(item) { ((Medicine *)item)->Allocate(this->sequence_++); }
      break;
    case entity::ItemType::MATERIAL:
      item = this->materials_.Allocate();
      if(item) { ((Material *)item)->Allocate(this->sequence_++); }
      break;
      break;
  /*
    case entity::ItemType::CARD:
      break;

    case entity::ItemType::TASK:
      break;
      */
    default:
      break;
  }
  return item;
}

void ItemPool::Deallocate(Item *item) {
  if(item == NULL) {
    return ;
  }
  switch(item->GetItemType()) {
    case entity::ItemType::EQUIPMENT:
      this->equipments_.Deallocate((Equipment *)item);
      break;
    case entity::ItemType::MEDICINE:
      ((Medicine *)item)->Deallocate();
      this->medicines_.Deallocate((Medicine *)item);
      break;
    case entity::ItemType::MATERIAL:
      this->materials_.Deallocate((Material *)item);
      break;
  /*
    case entity::ItemType::CARD:
      this->cards_.Deallocate((Medicine *)item);
    case entity::ItemType::TASK:
      this->tasks_.Deallocate((Medicine *)item);
      */
    default:
      break;
  }
}

}  // namespace item

}  // namespace server

}  // namespace game

