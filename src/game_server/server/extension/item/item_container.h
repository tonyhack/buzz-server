//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-20 12:01:30.
// File name: item_container.h
//
// Description:
// Define class ItemContainer.
//

#ifndef __GAME__SERVER__ITEM__ITEM__CONTAINER__H
#define __GAME__SERVER__ITEM__ITEM__CONTAINER__H

#include <string>
#include <vector>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "database_server/protocol/storage_item_login_types.h"
#include "database_server/protocol/storage_item_save_types.h"
#include "entity/item_types.h"
#include "entity/vocation_types.h"

namespace game {

namespace server {

namespace item {

class Item;
class ItemActor;

class ItemContainer : public core::Noncopyable {
 public:
  typedef std::vector<Item *> ItemVector;
  typedef std::vector<database::protocol::StorageItemField> ItemFieldVector;

  ItemContainer();
  virtual ~ItemContainer();

  bool Initialize(ItemActor *actor, core::uint16 capacity, core::uint16 max_capacity);
  void Finalize();

  // 初始化容器道具
  virtual bool InitializeItems(const ItemFieldVector &items);
  virtual void Save();

  bool ExportDatabase(database::protocol::StorageItemSaveRequest &data);

  virtual entity::ItemContainerType::type GetType() const = 0;

  // 容量/最大容量/当前个数
  inline core::uint16 GetCapacity() const {
    return this->capacity_;
  }
  inline core::uint16 GetMaxCapacity() const {
    return this->max_capacity_;
  }
  inline size_t GetSize() const {
    return this->items_.size();
  }

  // 获取道具
  inline Item *GetItem(core::uint16 index) const {
    if(index < this->GetSize()) {
      return this->items_[index];
    }
    return NULL;
  }
  // 获取道具
  inline Item *GetItem(core::uint16 index) {
    if(index < this->GetSize()) {
      return this->items_[index];
    }
    return NULL;
  }

  // 某个格子是否为空
  inline bool CheckLocationEmpty(core::uint16 location) const{
    if(location < this->GetSize()) {
      return this->items_[location] == NULL;
    }
    return true;
  }

  // 通过模板ID得到第一个道具
  Item *GetItemByTemplate(core::uint32 template_id);

  // 通过id得到道具
  Item *GetItemByID(core::uint64 id);

  // 得到指定位置的道具数目
  core::int32 GetItemNumByLocation(core::uint16 location) const;

  // 增加一个道具
  virtual bool CheckAddItem(Item *item, core::uint16 index) const;
  virtual bool AddItem(Item *item, core::uint16 index, bool sync = true, bool broadcast = true);

  virtual bool CheckAddItem(core::uint32 template_id, core::uint32 number, bool bind) const;
  virtual bool CheckAddItems(std::vector<entity::AddItemFields>& checklists) const;
  virtual bool AddItems(core::uint32 template_id, core::uint32 number, bool bind,
      core::int32 intensify_level, const std::string *extra_info = NULL);
  virtual bool AddItemsByLocation(core::uint16 location, core::uint32 template_id,
      core::uint32 number, bool simulate, bool bind, core::int32 intensify_level, const std::string *extra_info = NULL);

  // 移除一个道具
  virtual bool CheckRemoveItem(Item *item) const;
  virtual bool RemoveItem(Item *item, bool sync = true, bool broadcast = true);

  // 容器整理
  virtual void Arrange();

  // 扩容
  virtual void ExtendCapacity(core::int32 extend) {}

  // 找到一个空位
  core::uint16 GetEmptyPosition() const;

  // 空格数量
  core::uint16 GetEmptyNumber() const;

 protected:
  bool Assignment(Item *item, core::uint16 index, bool sync);

  void Synchronize();

  void Merge();

  // 整理排序函数
  static bool ArrageSorter(const Item *item1, const Item *item2);

  ItemActor *actor_;

  ItemVector items_;

  // 容量
  core::uint16 capacity_;
  // 最大容量
  core::uint32 max_capacity_;

  // 职业类型
  entity::VocationType::type vocation_;
};

}  // namespace item

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ITEM__ITEM__CONTAINER__H

