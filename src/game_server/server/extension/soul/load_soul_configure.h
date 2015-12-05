//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-14 16:56:07.
// File name: load_soul_configure.h
//
// Description:
// Define class LoadSoulConfigure.
//

#ifndef __GAME__SERVER__SOUL__LOAD__SOUL__CONFIGURE__H
#define __GAME__SERVER__SOUL__LOAD__SOUL__CONFIGURE__H

#include <ext/hash_map>

#include "core/base/noncopyable.h"
#include "game_server/server/extension/soul/soul_configure.h"

namespace game {

namespace server {

namespace soul {

class LoadSoulConfigure : public core::Noncopyable {
  typedef __gnu_cxx::hash_map<core::int32, SoulCell> SoulHashmap;

 public:
  LoadSoulConfigure();
  ~LoadSoulConfigure();

  bool LoadConfigure(const std::string &file);

  const SoulCell *GetSoul(core::uint32 id) const;

 private:
  SoulHashmap souls_;

  static const core::int32 kEveryLevel = 5;
};

}  // namespace soul

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SOUL__LOAD__SOUL__CONFIGURE__H

