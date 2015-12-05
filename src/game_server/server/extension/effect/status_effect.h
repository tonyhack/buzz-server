//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-24 19:38:23.
// File name: status_effect.h
//
// Description:
// Define class StatusEffect.
//

#ifndef __GAME__SERVER__EFFECT__STATUS__EFFECT__H
#define __GAME__SERVER__EFFECT__STATUS__EFFECT__H

#include "core/base/types.h"
#include "game_server/server/extension/effect/effect.h"
#include "game_server/server/extension/effect/effect_type.h"
#include "entity/role_types.h"

namespace game {

namespace server {

namespace effect {

class EffectRole;

class StatusEffect : public Effect {
 public:
  StatusEffect();
  virtual ~StatusEffect();

  virtual bool Initialize(EffectRole *role);
  virtual void Finalize();

  virtual bool Start(core::uint32 id);
  virtual bool End(core::uint32 id);

  virtual EffectType::type GetType() const {
    return EffectType::EFFECT_STATUS;
  }

  bool StartCommonStatus(EffectStatusType::type type);
  bool EndCommonStatus(EffectStatusType::type type);

  EffectStatusType::type ConvertType(entity::RoleCommonStatus::type type);

 private:
  void SetStatus(EffectStatusType::type type, bool add);

  // 一些状态的计数
  core::int32 forbid_move_;
  core::int32 forbid_skill_;
  core::int32 forbid_item_;
  core::int32 forbid_beaten_map_;
};

}  // namespace effect

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__EFFECT__STATUS__EFFECT__H

