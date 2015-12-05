//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-19 19:21:59.
// File name: load_soul_level_configure.h
//
// Description:
// Define class LoadSoulLevelConfigure.
//

#ifndef __GAME__SERVER__SOUL__LOAD__SOUL__LEVEL__CONFIGURE__H
#define __GAME__SERVER__SOUL__LOAD__SOUL__LEVEL__CONFIGURE__H

#include <ext/hash_map>

#include "core/base/noncopyable.h"
#include "game_server/server/extension/soul/soul_configure.h"

namespace game {

namespace server {

namespace soul {

class SoulLevelKey {
 public:
  SoulLevelKey(core::int32 group = 0, core::int32 level = 0)
    : group_(group), level_(level) {}

  inline bool operator== (const SoulLevelKey &key) const {
    return this->group_ == key.group_ && this->level_ == key.level_;
  }

  core::int32 group_;
  core::int32 level_;
};

class SoulLevelHashFunction {
 public:
  size_t operator()(const SoulLevelKey &key) const {
    size_t hash = key.group_;
    return (hash << 32) + key.level_;
  }
};

class LoadSoulLevelConfigure : public core::Noncopyable {
  typedef __gnu_cxx::hash_map<SoulLevelKey, SoulLevelCell,
          SoulLevelHashFunction> LevelHashmap;

 public:
  LoadSoulLevelConfigure();
  ~LoadSoulLevelConfigure();

  bool LoadConfigure(const std::string &file);

  const SoulLevelCell *GetLevel(core::int32 group, core::int32 level) const;

 private:
  LevelHashmap levels_;
};

}  // namespace soul

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SOUL__LOAD__SOUL__LEVEL__CONFIGURE__H

