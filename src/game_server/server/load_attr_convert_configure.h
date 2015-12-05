//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-10-28 14:40:18.
// File name: load_attr_convert_configure.h
//
// Description:
// Define class LoadAttrConvertConfigure.
//

#ifndef __GAME__SERVER__LOAD__ATTR__CONVERT__CONFIGURE__H
#define __GAME__SERVER__LOAD__ATTR__CONVERT__CONFIGURE__H

#include <map>
#include <string>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "entity/role_types.h"

namespace game {

namespace server {

class AttrConvertCell {
 public:
  AttrConvertCell() {}
  ~AttrConvertCell() {}

  // 最大生命值/魔法值
  core::int32 max_hp_;
  core::int32 max_mp_;
  // 物理攻击/防御
  core::int32 physical_attack_;
  core::int32 physical_defence_;
  // 魔法攻击/防御
  core::int32 magic_attack_;
  core::int32 magic_defence_;
  // 闪避/暴击/会心
  core::int32 dodge_;
  core::int32 crit_;
  core::int32 ignore_defence_;
};

class LoadAttrConvertConfigure : public core::Noncopyable {
  typedef std::map<entity::RoleClientFields::type, AttrConvertCell> CellMap;

 public:
  LoadAttrConvertConfigure();
  ~LoadAttrConvertConfigure();

  bool LoadConfigure(const std::string &file);

  const AttrConvertCell *GetCell(entity::RoleClientFields::type type) const;

 private:
  CellMap cells_;
};

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__LOAD__ATTR__CONVERT__CONFIGURE__H

