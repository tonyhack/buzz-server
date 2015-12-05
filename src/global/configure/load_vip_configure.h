//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2014-02-16 05:14:16.
// File name: load_vip_configure.h
//
// Description:
//
#ifndef __GLOBAL__LOAD__VIP__CONFIGURE__H
#define __GLOBAL__LOAD__VIP__CONFIGURE__H

#include <string>
#include <vector>
#include <map>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "entity/vip_types.h"

namespace global {

namespace configure {

class VipCell {
 public:
  VipCell() {}
  ~VipCell() {}

  core::int32 value1_;
  core::int32 value2_;
};

class VipCells {
  typedef std::map<core::int32, VipCell> VipCellMap;

 public:
  VipCells() {}
  ~VipCells() {}

  const VipCell *GetCell() const {
    VipCellMap::const_iterator iterator = this->cells_.begin();
    if (iterator != this->cells_.end()) {
      return &iterator->second;
    }
    return NULL;
  }
  const VipCell *GetCell(core::int32 key) const {
    VipCellMap::const_iterator iterator = this->cells_.find(key);
    if (iterator != this->cells_.end()) {
      return &iterator->second;
    }
    return NULL;
  }

  VipCellMap cells_;
};

class LoadVipConfigure : public core::Noncopyable {
 public:
  typedef std::pair<entity::VipType::type, entity::VipPrivilegeType::type> Key;
  typedef std::map<Key, VipCells> VipMap;

  LoadVipConfigure();
  ~LoadVipConfigure();

  bool LoadConfigure(const std::string &file);

  const VipCell *GetCell(entity::VipType::type vip_type,
      entity::VipPrivilegeType::type privilege) const;
  const VipCell *GetCell(entity::VipType::type vip_type,
      entity::VipPrivilegeType::type privilege, core::int32 key) const;

 private:
  VipMap privileges_;
};

}  // namespace global

}  // namespace configure

#endif  // __GLOBAL__LOAD__VIP__CONFIGURE__H

