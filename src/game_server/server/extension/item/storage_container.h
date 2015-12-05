//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-23 20:29:45.
// File name: storage_container.h
//
// Description:
// Define class StorageContainer.
//

#ifndef __GAME__SERVER__ITEM__STORAGE__CONTAINER__H
#define __GAME__SERVER__ITEM__STORAGE__CONTAINER__H

#include "entity/item_types.h"
#include "game_server/server/extension/item/item_container.h"

namespace game {

namespace server {

namespace item {

class ItemActor;

class StorageContainer : public ItemContainer {
 public:
  using ItemContainer::CheckAddItem;

  StorageContainer();
  virtual ~StorageContainer();

  bool Initialize(ItemActor *actor, core::uint16 capacity, core::uint16 max_capacity);
  void Finalize();

  inline bool CheckLoadFinish() const {
    return this->load_finish_;
  }

  inline void SetLoadFinish(bool finish = true) {
    this->load_finish_ = finish;
  }

  virtual entity::ItemContainerType::type GetType() const {
    return entity::ItemContainerType::STORAGE;
  }

  virtual void ExtendCapacity(core::int32 extend);

 private:
  bool load_finish_;
};

}  // namespace item

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ITEM__STORAGE__CONTAINER__H

