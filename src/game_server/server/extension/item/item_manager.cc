//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-20 14:17:30.
// File name: item_manager.cc
//
// Description:
// Define class ItemManager.
//

#include "game_server/server/extension/item/item_manager.h"

#include "game_server/server/extension/item/item.h"

namespace game {

namespace server {

namespace item {

Item *ItemManager::Get(core::uint64 id) {
  ItemHashmap::iterator iterator = this->items_.find(id);
  if(iterator != this->items_.end()) {
    return iterator->second;
  }
  return NULL;
}

bool ItemManager::Add(Item *item) {
  // 检测指针是否合法
  if(item == NULL) {
    return false;
  }
  // 检测是否存在
  ItemHashmap::iterator iterator = this->items_.find(item->GetID());
  if(iterator != this->items_.end()) {
    return false;
  }
  // 加入管理器中
  this->items_.insert(std::make_pair(item->GetID(), item));
  return true;
}

Item *ItemManager::Remove(core::uint64 id) {
  Item *item = NULL;
  ItemHashmap::iterator iterator = this->items_.find(id);
  if(iterator != this->items_.end()) {
    item = iterator->second;
    this->items_.erase(iterator);
  }
  return item;
}

}  // namespace item

}  // namespace server

}  // namespace game

