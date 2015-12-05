//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-09-05 23:51:32.
// File name: shop_container.cc
//
// Description:
//

#include "game_server/server/extension/item/shop_buyback_container.h"

#include "database_server/client/database_client.h"
#include "database_server/protocol/storage_item_save_types.h"
#include "database_server/protocol/storage_item_login_types.h"
#include "game_server/server/extension/item/item_actor.h"
#include "game_server/server/extension/item/item_builder.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace item {


ShopBuybackContainer::ShopBuybackContainer() {}
ShopBuybackContainer::~ShopBuybackContainer() {}


bool ShopBuybackContainer::Initialize(ItemActor *actor, core::int16 capatity_max) {
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 参数 actor 不合法",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(ItemContainer::Initialize(actor, capatity_max, capatity_max) == false) {
    LOG_ERROR("玩家(%lu)商店回购列表失败", actor->GetID());
    return false;
  }
  this->location_ = 0;
  return true;
}

void ShopBuybackContainer::Finalize() {
  this->actor_ = NULL;
  this->location_ = 0;
  ItemContainer::Finalize();
}

bool ShopBuybackContainer::AddItem(Item *item) {
  if(item == NULL) {
    LOG_ERROR("参数传递错误");
    return false;
  }
  if(this->location_ == 0) {
    if(ItemContainer::AddItem(item, this->location_, false) == false) {
      LOG_ERROR("商品回购时，位置有问题[%d]", this->location_);
      return false;
    }
    ++this->location_;
  } else {
    if(this->location_ > this->max_capacity_) {
      LOG_ERROR("商品回购列表下标超出(%d)", this->location_);
      return false;
    } else if(this->location_ == this->max_capacity_) {
      Item *delete_item = this->GetItem(this->location_ - 1);
      if(delete_item == NULL) {
        return false; 
      }
      ItemContainer::RemoveItem(delete_item, false, false);
      ItemBuilder::GetInstance()->Destory(delete_item);
      --this->location_;
    } else {

    }
    core::int32 i = this->location_ - 1;
    while(i >= 0) {
      Item *move = this->GetItem(i);
      if(move == NULL) {
        return false;
      }
      ItemContainer::RemoveItem(move, false, false);
      ItemContainer::AddItem(move, i + 1, false);
      --i;
    }
    ++location_;
    return ItemContainer::AddItem(item, 0, false);
  }
  return true;
}

Item *ShopBuybackContainer::RemoveItem(core::int16 location) {
  if((core::uint16)location >= this->location_) {
    LOG_ERROR("参数错误");
    return NULL;
  }

  Item *item = GetItem(location);
  if(item == NULL) {
    return NULL;
  }
  if(ItemContainer::RemoveItem(item, false, false) == false) {
    LOG_ERROR("删除回购列表中的道具失败(%d)", location);
    return NULL;
  }
  while((core::uint32)location < this->location_ - 1) {
    Item *swap_item = this->GetItem(location + 1);
    if(swap_item == NULL) {
      return item;
    }
    ItemContainer::RemoveItem(swap_item, false, false);
    ItemContainer::AddItem(swap_item, location, false);
    ++location;
  }
  --this->location_;

  return item;
}

}   // namespace item

}   // namespace server

}   // namespace game
