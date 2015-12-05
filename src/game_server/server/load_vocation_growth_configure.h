//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-10-28 15:16:59.
// File name: load_vocation_growth_configure.h
//
// Description:
// Define class LoadVocationGrowthConfigure.
//

#ifndef __GAME__SERVER___LOAD__VOCATION__GROWTH__CONFIGURE__H
#define __GAME__SERVER___LOAD__VOCATION__GROWTH__CONFIGURE__H

#include <map>
#include <string>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "entity/vocation_types.h"

namespace game {

namespace server {

class VocationGrowthCell {
 public:
  VocationGrowthCell() {}
  ~VocationGrowthCell() {}

  // 体、力、智、敏、精成长倍数(放大100倍)
  core::int32 physique_;
  core::int32 strength_;
  core::int32 intelligence_;
  core::int32 agile_;
  core::int32 spirit_;
};

class LoadVocationGrowthConfigure : public core::Noncopyable {
  typedef std::map<entity::VocationType::type, VocationGrowthCell> CellMap;

 public:
  LoadVocationGrowthConfigure();
  ~LoadVocationGrowthConfigure();

  bool LoadConfigure(const std::string &file);

  const VocationGrowthCell *GetCell(entity::VocationType::type type) const;

 private:
  CellMap cells_;
};

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER___LOAD__VOCATION__GROWTH__CONFIGURE__H

