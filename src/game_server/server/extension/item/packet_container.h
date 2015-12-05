//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-21 14:32:27.
// File name: packet_container.h
//
// Description:
// Define class PacketContainer.
//

#ifndef __GAME__SERVER__ITEM__PACKET__CONTAINER__H
#define __GAME__SERVER__ITEM__PACKET__CONTAINER__H

#include "entity/item_types.h"
#include "game_server/server/extension/item/item_container.h"

namespace game {

namespace server {

namespace item {

class ItemActor;

class PacketContainer : public ItemContainer {
 public:
  using ItemContainer::CheckAddItem;
  using ItemContainer::RemoveItem;

  PacketContainer();
  virtual ~PacketContainer();

  bool Initialize(ItemActor *actor, core::uint16 capacity, core::uint16 max_capacity);
  void Finalize();
  
  inline bool CheckLoadFinish() const {
    return this->load_finish_;
  }

  inline void SetLoadFinish(bool finish = true) {
    this->load_finish_ = finish;
  }

  virtual entity::ItemContainerType::type GetType() const {
    return entity::ItemContainerType::PACKET;
  }

  virtual void ExtendCapacity(core::int32 extend);

  // 检测道具是否存在
  // 0: 非绑定 1: 绑定 2: 两者都可以
  core::uint32 GetItemNumber(core::uint32 template_id,
      entity::BindRequireType::type bind);

  // 删除道具
  core::uint32 RemoveItem(core::uint32 template_id, core::uint32 number,
      entity::BindRequireType::type &bind);

 private:
  bool load_finish_;
};

}  // namespace item

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ITEM__PACKET__CONTAINER__H

