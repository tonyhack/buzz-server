//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-11-21 19:40:07.
// File name: load_rune_configure.h
//
// Description:
//
#ifndef __GAME__SERVER__ATTR__LOAD__RUNE__CONFIGURE__H
#define __GAME__SERVER__ATTR__LOAD__RUNE__CONFIGURE__H

#include <cstddef>
#include <string>
#include <vector>
#include <ext/hash_map>

#include "entity/spirit_types.h"
#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "game_server/server/extension/attr/attr_configure.h"

namespace game {

namespace server {

namespace attr {

class RuneCell {
 public:
  core::int32 template_id_;
  entity::RuneAttributeType::type type_1_;
  core::int32 attr_1_;
  core::int32 num_;
  core::int32 fuse_template_;
  core::int32 odds_;
};


class LoadRuneConfigure : public core::Noncopyable {
  typedef __gnu_cxx::hash_map<core::int32, RuneCell> RuneHashmap;

 public:
  LoadRuneConfigure();
  ~LoadRuneConfigure();

  bool LoadConfigure(const std::string &file);

  const RuneCell *GetRune(core::int32 id) const;

 private:
  RuneHashmap cells_;
};

}  // namespace attr

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ATTR__LOAD__RUNE__CONFIGURE__H

