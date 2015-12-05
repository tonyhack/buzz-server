#ifndef __GAME__SERVER__CHAT__FACADE__REQUEST__H
#define __GAME__SERVER__CHAT__FACADE__REQUEST__H

#include "core/base/types.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace chat {

struct BindType {
  enum type {
    NOT_BIND = 0,
    BIND = 1,
    BOTH = 2,
  };
};

class FacadeRequest : public global::SingletonFactory<FacadeRequest> {
  friend class global::SingletonFactory<FacadeRequest>;

 public:
  // 物品是否存在于背包
  bool CheckPacketItemExist(core::uint64 actor, core::uint32 item_template,
      core::uint32 number, BindType::type bind);
  // 背包删除物品
  core::uint32 RemovePacketItem(core::uint64 actor, core::uint32 item_template,
      core::uint32 number, BindType::type bind);

 private:
  FacadeRequest() {}
  ~FacadeRequest() {}
};

}  // namespace chat

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__CHAT__FACADE__REQUEST__H

