//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-03-13 23:11:43.
// File name: load_soul_skill_level_configure.h
//
// Description:
// Define class LoadSoulSkillLevelConfigure.
//

#ifndef __GAME__SERVER__SOUL__LOAD__SOUL__SKILL__LEVEL__CONFIGURE__H
#define __GAME__SERVER__SOUL__LOAD__SOUL__SKILL__LEVEL__CONFIGURE__H

#include <ext/hash_map>

#include "core/base/noncopyable.h"
#include "game_server/server/extension/soul/soul_configure.h"

namespace game {

namespace server {

namespace soul {

class LoadSoulSkillLevelConfigure : public core::Noncopyable {
  typedef __gnu_cxx::hash_map<core::int32, SoulSkillLevelCell> SkillLevelHashmap;

 public:
  LoadSoulSkillLevelConfigure();
  ~LoadSoulSkillLevelConfigure();

  bool LoadConfigure(const std::string &file);

  const SoulSkillLevelCell *GetSkillLevel(core::int32 skill_level) const;

 private:
  SkillLevelHashmap levels_;
};

}  // namespace soul

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SOUL__LOAD__SOUL__SKILL__LEVEL__CONFIGURE__H

