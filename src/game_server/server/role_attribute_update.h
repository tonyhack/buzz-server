//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-12 14:29:26.
// File name: role_attribute_update.h
//
// Description:
// Define class RoleAttributeUpdate.
//

#ifndef __GAME__SERVER__ROLE__ATTRIBUTE__UPDATE__H
#define __GAME__SERVER__ROLE__ATTRIBUTE__UPDATE__H

#include "core/base/noncopyable.h"
#include "entity/role_types.h"

namespace game {

namespace server {

class GameRole;

class RoleAttributeUpdate : public core::Noncopyable {
 public:
  RoleAttributeUpdate(GameRole *role);
  ~RoleAttributeUpdate();

  bool Initialize();
  void Finalize();

  void UpdateAll(bool synchronize = false);

  void Update(entity::RoleAoiFields::type type, bool synchronize = false);
  void Update(entity::RoleClientFields::type type, bool synchronize = false);
  void Update(entity::RoleServerFields::type type, bool synchronize = false);

 private:
  // 更新状态
  void UpdateStatus(bool synchronize);
  // 更新血量值
  void UpdateHp(bool synchronize);
  // 更新魔法值
  void UpdateMp(bool synchronize);
  // 更新最大血量值
  void UpdateMaxHp(bool synchronize);
  // 更新最大魔法值
  void UpdateMaxMp(bool synchronize);
  // 更新移动速度
  void UpdateMoveSpeed(bool synchronize);
  // 更新体质
  void UpdatePhysique(bool synchronize);
  // 更新力量
  void UpdateStrength(bool synchronize);
  // 更新智力
  void UpdateIntelligence(bool synchronize);
  // 更新敏捷
  void UpdateAgile(bool synchronize);
  // 更新精神
  void UpdateSpirit(bool synchronize);
  // 更新物理攻击
  void UpdatePhysicsAttack(bool synchronize);
  // 更新物理防御
  void UpdatePhysicsDefence(bool synchronize);
  // 更新魔法攻击
  void UpdateMagicAttack(bool synchronize);
  // 更新魔法防御
  void UpdateMagicDefence(bool synchronize);
  // 更新闪避等级
  void UpdateDodge(bool synchronize);
  // 更新暴击等级
  void UpdateCrit(bool synchronize);
  // 更新会心等级(忽视防御)
  void UpdateIgnoreDefence(bool synchronize);
  // 更新当前能量值
  void UpdateCurrentEnergy(bool synchronize);
  // 更新最大能量值
  void UpdateMaxEnergy(bool synchronize);
  // 更新单位时间能量恢复值
  void UpdateEnergyIncrement(bool synchronize);

  GameRole *role_;
};

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ROLE__ATTRIBUTE__UPDATE__H

