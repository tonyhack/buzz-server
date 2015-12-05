
#ifndef __GAME__SERVER__ITEM__TREASURE__CONTAINER__H
#define __GAME__SERVER__ITEM__TREASURE__CONTAINER__H

#include "entity/item_types.h"
#include "game_server/server/extension/item/item_container.h"

namespace game {

namespace server {

namespace item {

class ItemActor;

class TreasureContainer : public ItemContainer {
 public:
  using ItemContainer::CheckAddItem;
  using ItemContainer::RemoveItem;

  TreasureContainer();
  virtual ~TreasureContainer();

  bool Initialize(ItemActor *actor, core::uint16 capacity, core::uint16 max_capacity);
  void Finalize();

  inline bool CheckLoadFinish() const {
    return this->load_finish_;
  }

  inline void SetLoadFinish(bool finish = true) {
    this->load_finish_ = finish;
  }

  virtual entity::ItemContainerType::type GetType() const {
    return entity::ItemContainerType::TREASURE;
  }

  virtual void ExtendCapacity(core::int32 extend);

  // 禁用函数
  virtual bool AddItemsByLocation(core::uint16 location, core::uint32 template_id,
      core::uint32 number, bool simulate, bool bind, core::int32 intensify_level, const std::string *extra_info = NULL) {
    return false;
  }

  // 删除道具
  core::uint32 RemoveItem(core::uint32 template_id, core::uint32 number,
      entity::BindRequireType::type &bind);

 private:
  bool load_finish_;
};

}  // namespace item

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ITEM__TREASURE__CONTAINER__H

