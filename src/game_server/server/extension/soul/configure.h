//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-14 19:11:06.
// File name: configure.h
//
// Description:
// Define class Configure.
//

#ifndef __GAME__SERVER__SOUL__CONFIGURE__H
#define __GAME__SERVER__SOUL__CONFIGURE__H

#include <string>

#include "core/base/types.h"
#include "game_server/server/extension/soul/soul_configure.h"
#include "game_server/server/extension/soul/load_soul_configure.h"
#include "game_server/server/extension/soul/load_soul_level_configure.h"
#include "game_server/server/extension/soul/load_soul_skill_level_configure.h"
#include "game_server/server/extension/soul/load_soul_step_configure.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace soul {

class Configure : public global::SingletonFactory<Configure> {
  friend class global::SingletonFactory<Configure>;

 public:
  bool LoadConfigure(const std::string &file);

  inline const SoulCell *GetSoulCell(core::uint32 id) const {
    return this->load_soul_.GetSoul(id);
  }
  inline const SoulLevelCell *GetSoulLevelCell(core::int32 group, core::int32 level) const {
    return this->load_soul_level_.GetLevel(group, level);
  }
  inline const SoulSkillLevelCell *GetSoulSkillLevel(core::int32 skill_level) const {
    return this->load_skill_soul_level_.GetSkillLevel(skill_level);
  }
  inline const SoulStepCell *GetSoulStepCell(core::int32 group, core::int32 level,
      core::int32 step) const {
    return this->load_soul_step_.GetStep(group, level, step);
  }

  core::uint32 soul_pool_initial_;
  core::uint32 soul_pool_extend_;

 private:
  Configure() {}
  ~Configure() {}

  LoadSoulConfigure load_soul_;
  LoadSoulLevelConfigure load_soul_level_;
  LoadSoulSkillLevelConfigure load_skill_soul_level_;
  LoadSoulStepConfigure load_soul_step_;
};

}  // namespace soul

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SOUL__CONFIGURE__H

