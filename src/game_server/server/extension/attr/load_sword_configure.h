//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-08-07 10:06:11.
// File name: load_sword_configure.h
//
// Description:
//

#ifndef __GAME__SERVER__ATTR__LOAD__SWORD__CONFIGURE__H
#define __GAME__SERVER__ATTR__LOAD__SWORD__CONFIGURE__H

#include <map>
#include <string>
#include <ext/hash_map>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "game_server/server/extension/attr/attr_configure.h"

namespace game {

namespace server {

namespace attr {

class LoadSwordConfigure : public core::Noncopyable {
  typedef std::map<core::int32, SwordCell> SwordMap;
  typedef __gnu_cxx::hash_map<core::int32, SwordLevelCell> SwordLevelHashmap;

 public:
  LoadSwordConfigure();
  ~LoadSwordConfigure();

  bool LoadConfigure(const std::string &file);
  bool LoadLevelConfigure(const std::string &file);

  const SwordCell *GetSword(core::int32 id) const;
  const SwordLevelCell *GetLevel(core::int32 level) const;

 private:
  SwordMap swords_;
  SwordLevelHashmap levels_;
};

}  // namespace attr

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ATTR__LOAD__SWORD__CONFIGURE__H

