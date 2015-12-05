//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-09-05 23:22:10.
// File name: shop_container.h
//
// Description:
//

#ifndef __GAME__SERVER__ITEM__SHOP__BUYBACK__CONTAINER__H
#define __GAME__SERVER__ITEM__SHOP__BUYBACK__CONTAINER__H

#include "core/base/types.h"
#include "entity/item_types.h"
#include "database_server/protocol/storage_item_login_types.h"
#include "game_server/server/extension/item/item_container.h"

namespace game {

namespace server {

namespace item {

class ItemActor;

class ShopBuybackContainer : public ItemContainer {
 public:

  ShopBuybackContainer();
   ~ShopBuybackContainer();

  bool Initialize(ItemActor *actor, core::int16 capacity_max);
  void Finalize();


  virtual entity::ItemContainerType::type GetType() const {
     return entity::ItemContainerType::SHOP;
  }

  inline void SetLocation(core::int16 location) {
    if(location >= this->capacity_ || location < 0) {
      location_ = 0;
    } else {
      location_ = location;
    }
    return ;
  }
  // 添加道具
  bool AddItem(Item *item);
  // 移除一个道具
  Item *RemoveItem(core::int16 location);

  virtual bool CheckAddItem(Item *item, core::uint16 index) const {
    if(item == NULL) {
       return false;
    }
    return true;
  }

  // 禁用函数
  virtual bool CheckAddItem(core::uint32 template_id, core::uint32 number, bool bind) const {
    return false;
  }
  virtual bool CheckAddItems(std::vector<entity::AddItemFields>& checklists) const {
    return false;
  }
  virtual bool AddItems(core::uint32 template_id, core::uint32 number, bool bind,
      core::int32 intensify_level, const std::string *extra_info = NULL) {
    return false;
  }
  virtual bool AddItemsByLocation(core::uint16 location, core::uint32 template_id,
      core::uint32 number, bool simulate, bool bind, core::int32 intensify_level, const std::string *extra_info = NULL) {
    return false;
  }

  virtual void Arrange() {}
  
 private:
  core::uint32 location_;
};

}  // namespace item

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ITEM__SHOP__BUYBACK__CONTAINER__H

