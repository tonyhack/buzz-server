#ifndef __GAME__SERVER__GUILD__FACADE__REQUEST__H
#define __GAME__SERVER__GUILD__FACADE__REQUEST__H

#include "core/base/types.h"
#include "entity/item_types.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace guild {

class FacadeRequest : public global::SingletonFactory<FacadeRequest> {
  friend class global::SingletonFactory<FacadeRequest>;

 public:
  // 物品是否存在于背包
  bool CheckPacketItemExist(core::uint64 actor, core::uint32 item_template,
      core::uint32 number, entity::BindRequireType::type bind);
  // 背包删除物品
  core::uint32 RemovePacketItem(core::uint64 actor, core::uint32 item_template,
      core::uint32 number, entity::BindRequireType::type bind);
  // 检查背包空位
  bool CheckAddPacketItem(core::uint64 actor, core::uint32 item_template,
      core::uint32 number, entity::BindRequireType::type bind);
  bool CheckAddPacketItems(core::uint64 actor,
      const std::vector<entity::AddItemFields> &items);
  // 背包增加物品
  bool AddPacketItem(core::uint64 actor, core::uint32 item_template,
      core::uint32 number, entity::BindRequireType::type bind);
  bool AddPacketItems(core::uint64 actor,
      const std::vector<entity::AddItemFields> &items);
  // 给角色增加一个BUFF
  bool AddBuff(core::uint64 actor, core::uint32 buff);
  // 给角色移除一个BUFF
  bool RemoveBuff(core::uint64 actor, core::uint32 buff);

 private:
  FacadeRequest() {}
  ~FacadeRequest() {}
};

}  // namespace guild

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__GUILD__FACADE__REQUEST__H

