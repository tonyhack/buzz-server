#ifndef __GAME__SERVER__ATTR__LOAD__LEGENDARY__WEAPON__CONFIGURE__H
#define __GAME__SERVER__ATTR__LOAD__LEGENDARY__WEAPON__CONFIGURE__H

#include <string>
#include <ext/hash_map>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "game_server/server/extension/attr/attr_configure.h"

namespace game {

namespace server {

namespace attr {

class LoadLegendaryWeaponConfigure : public core::Noncopyable {
 public:
  typedef __gnu_cxx::hash_map<core::int32, LegendaryWeaponCell> LegendaryWeaponHashmap;

  LoadLegendaryWeaponConfigure();
  ~LoadLegendaryWeaponConfigure();

  bool LoadConfigure(const std::string &file);

  const LegendaryWeaponCell *GetLegendaryWeapon(core::int32 id) const;

 private:
  LegendaryWeaponHashmap weapons_;
};

}  // namespace attr

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ATTR__LOAD__LEGENDARY__WEAPON__CONFIGURE__H

