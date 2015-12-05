//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-20 10:40:55.
// File name: item.h
//
// Description:
// Define class Item.
//

#ifndef __GAME__SERVER__ITEM__ITEM__H
#define __GAME__SERVER__ITEM__ITEM__H

#include <vector>

#include "core/base/types.h"
#include "database_server/protocol/storage_item_login_types.h"
#include "entity/item_types.h"
#include "game_server/server/game_object.h"

namespace game {

namespace server {

namespace item {

class ItemActor;

class Item : public GameObject {
  typedef std::vector<int32_t> AttributeFields;

  typedef void (*AttributeReviseFunctor)(core::int32 &value);

 public:
  Item();
  virtual ~Item();

  void Allocate(core::uint64 id);
  void Deallocate();

  virtual bool Initialize(core::uint32 template_id, ItemActor *actor);
  virtual void Finalize();

  // 从数据库初始化
  virtual void InitializeDatabase(const database::protocol::StorageItemField &field) = 0;

  // 导出数据库存储字段
  virtual void ExportDatabaseField(database::protocol::StorageItemField &field) = 0;

  // 导出初始化字段
  virtual void ExportInitialField(entity::ItemFields &field) const = 0;

  // 导入道具额外信息
  virtual void ImportExtraInfo(const std::string &input) {}
  // 导出道具额外信息
  virtual void ExportExtraInfo(std::string &output) const {}

  // 道具类型
  virtual entity::ItemType::type GetItemType() const = 0;

  // 私有可见属性
  core::int32 GetAttribute(entity::ItemClientFields::type type) const;
  bool SetAttribute(entity::ItemClientFields::type type, core::int32 value,
      bool synchronize = true);

  // 服务器属性
  core::int32 GetAttribute(entity::ItemServerFields::type type) const;
  bool SetAttribute(entity::ItemServerFields::type type, core::int32 value,
      bool synchronize = true);

  inline const AttributeFields & GetItemClientAttributes() const {
    return this->client_attribute_;
  }

  // 同步
  virtual void Synchronize() = 0;
  virtual void Synchronize(entity::ItemClientFields::type type);

  // 道具是否过期
  virtual bool CheckExpire() const { return false; }

  inline void SetActor(ItemActor *actor) {
    this->actor_ = actor;
  }

  // 得到模板ID
  inline core::uint32 GetTemplateID() const {
    return this->template_id_;
  }

  // 设置道具所在容器和位置
  inline void SetContainerLocation(entity::ItemContainerType::type type,
      core::uint16 location) {
    this->SetContainer(type);
    this->SetLocation(location);
  }

  // 设置/获取 道具所在容器
  inline void SetContainer(entity::ItemContainerType::type type) {
    this->container_type_ = type;
  }
  inline entity::ItemContainerType::type GetContainer() const {
    return this->container_type_;
  }

  // 设置/获取 道具在位置
  inline void SetLocation(core::uint16 location) {
    this->location_ = location;
  }
  inline core::uint16 GetLocation() const {
    return this->location_;
  }

  // 得到道具的绑定属性
  inline bool GetBind() const {
    return this->GetAttribute(entity::ItemClientFields::BIND_TYPE)
      == 0 ? false : true;
  }

  // 得到道具数目属性
  inline core::int32 GetCount() const {
    return this->GetAttribute(entity::ItemClientFields::STACK_COUNT);
  }

 protected:
  // 修正属性
  void ReviseAttribute(entity::ItemClientFields::type type);
  void ReviseAttribute(entity::ItemServerFields::type type);

  ItemActor *actor_;

  // 模板ID
  core::uint32 template_id_;

  AttributeFields client_attribute_;
  AttributeFields server_attribute_;

  // 所在容器类型
  entity::ItemContainerType::type container_type_;

  // 在容器中的位置
  core::uint16 location_;
};

}  // namespace item

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ITEM__ITEM__H

