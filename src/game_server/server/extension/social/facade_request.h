#ifndef __GAME__SERVER__SOCIAL__FACADE__REQUEST__H
#define __GAME__SERVER__SOCIAL__FACADE__REQUEST__H

#include "core/base/types.h"
#include "entity/item_types.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace social {

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
  // 背包增加物品
  bool AddPacketItem(core::uint64 actor, core::uint32 item_template,
      core::uint32 number, entity::BindRequireType::type bind);

 private:
  FacadeRequest() {}
  ~FacadeRequest() {}
};

}  // namespace social

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SOCIAL__FACADE__REQUEST__H

