//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-20 10:30:18.
// File name: facade_request.h
//
// Description:
// Define class FacadeRequest.
//

#ifndef __GAME__SERVER__SOUL__FACADE__REQUEST__H
#define __GAME__SERVER__SOUL__FACADE__REQUEST__H

#include "core/base/types.h"
#include "entity/item_types.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace soul {

class FacadeRequest : public global::SingletonFactory<FacadeRequest> {
  friend class global::SingletonFactory<FacadeRequest>;

 public:
  // 物品是否存在于背包
  bool CheckPacketItemExist(core::uint64 actor, core::uint32 item_template,
      core::uint32 number, entity::BindRequireType::type bind);

  // 从背包删除物品
  core::uint32 RemovePacketItem(core::uint64 actor, core::uint32 item_template,
      core::uint32 number, entity::BindRequireType::type bind);

  // 检查背包空位
  bool CheckAddPacketItem(core::uint64 actor, core::uint32 item_template,
      core::uint32 number, entity::BindRequireType::type bind);

  // 背包增加物品
  bool AddPacketItem(core::uint64 actor, core::uint32 item_template,
      core::uint32 number, entity::BindRequireType::type bind);

  // 升级玩家技能
  bool UpgradeActorSkill(core::uint64 actor, core::uint32 skill_id,
      core::int32 upgrade);

  // 删除玩家技能
  bool RemoveActorSkill(core::uint64 actor, core::uint32 skill_id);

  // 增加玩家技能
  bool AddActorSkill(core::uint64 actor, core::uint32 skill_id, core::uint32 level,
      bool stand = false);

  // 启用技能
  bool StandActorSkill(core::uint64 actor, core::uint32 skill_id);

  // 收回技能
  bool SitActorSkill(core::uint64 actor, core::uint32 skill_id);

  // 获取魔剑阶级
  core::int32 GetSwordStep(core::uint64 actor);

  // 获取精灵等级
  core::int32 GetSpiritLevel(core::uint64 actor);

  // 获取贵族(爵位)等级
  core::int32 GetNobleLevel(core::uint64 actor);

  // 给角色增加一个BUFF
  bool AddBuff(core::uint64 actor, core::uint32 buff);

  // 给角色移除一个BUFF
  bool RemoveBuff(core::uint64 actor, core::uint32 buff);

 private:
  FacadeRequest() {}
  ~FacadeRequest() {}
};

}  // namespace soul

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SOUL__FACADE__REQUEST__H

