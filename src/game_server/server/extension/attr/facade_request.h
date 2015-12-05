#ifndef __GAME__SERVER__ATTR__FACADE__REQUEST__H
#define __GAME__SERVER__ATTR__FACADE__REQUEST__H

#include "core/base/types.h"
#include "game_server/server/extension/attr/attr_configure.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace attr {

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

  // 增加技能全局BUFF
  bool AddSkillGlobalDebuff(core::uint64 actor, const SwordCell::BuffVector &buffs);
  // 删除技能全局BUFF
  bool RemoveSkillGlobalDebuff(core::uint64 actor, const SwordCell::BuffVector &buffs);

  // 给角色增加一个BUFF
  bool AddBuff(core::uint64 actor, core::uint32 buff);
  // 给角色移除一个BUFF
  bool RemoveBuff(core::uint64 actor, core::uint32 buff);

 private:
  FacadeRequest() {}
  ~FacadeRequest() {}
};

}  // namespace attr

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ATTR__FACADE__REQUEST__H

