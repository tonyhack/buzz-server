//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-24 12:01:35.
// File name: soul_setting.h
//
// Description:
// Define class SoulSetting.
//

#ifndef __GAME__SERVER__SOUL__SOUL__SETTING__H
#define __GAME__SERVER__SOUL__SOUL__SETTING__H

#include "core/base/types.h"
#include "entity/soul_types.h"
#include "entity/vocation_types.h"
#include "game_server/server/extension/soul/soul_types.h"

namespace game {

namespace server {

namespace soul {

class Soul;
class SoulActor;

class SoulSetting {
 public:
  typedef std::vector<int32_t> SettingVector;

  SoulSetting();
  ~SoulSetting();

  bool Initialize(SoulActor *actor,
      entity::VocationType::type vocation, core::int32 group, size_t size);
  void Finalize();

  bool Stand();
  bool Sit();

  bool SetSoul(Soul *soul, size_t pos);
  bool ResetSoul(size_t pos);

  core::uint32 GetSoul(size_t pos) const;
  const SettingVector &GetSettings() const;

  inline entity::VocationType::type GetVocation() const {
    return this->vocation_;
  }

 private:
  SoulActor *actor_;

  bool stand_status_;

  entity::VocationType::type vocation_;
  core::int32 group_;
  SettingVector settings_;
};

}  // namespace soul

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SOUL__SOUL__SETTING__H

