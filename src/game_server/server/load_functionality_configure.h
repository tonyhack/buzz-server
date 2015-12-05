//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-12-02 11:05:37.
// File name: load_functionality_configure.h
//
// Description:
//
#ifndef __GAME__SERVER__LOAD__FUNCTIONALITY__CONFIGURE__H
#define __GAME__SERVER__LOAD__FUNCTIONALITY__CONFIGURE__H

#include <map>
#include <string>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "entity/functionality_types.h"

namespace game {

namespace server {

class FunctionalityOpenCell {
 public:
  entity::FunctionalityType::type function_;
  entity::FunctionalityOpenType::type open_type_;
  core::int32 param_;
  core::int32 id_;
  std::string func_str;
};

class LoadFunctionalityConfigure {
 public:
  typedef std::vector<FunctionalityOpenCell> OpenCells;
  typedef std::map<core::int32, std::string> OpenCellMaps;

  LoadFunctionalityConfigure();
  ~LoadFunctionalityConfigure();

  bool LoadConfigure(const std::string& file);

  OpenCells& GetCell(entity::FunctionalityOpenType::type type);

  const char *GetFunctionalityStr(core::int32 id);

 private:
  OpenCells level_cells_;
  OpenCells task_accept_cells_;
  OpenCells task_finish_cells_;
  OpenCells spirit_level_cells_;
  OpenCells use_item_cells_;
  OpenCells client_request_cells_;
  OpenCells homeland_temple_level_cells_;
  OpenCellMaps open_maps_;
};

} // server

} // game

#endif  // GAME__SERVER__LOAD__FUNCTIONALITY__CONFIGURE__H
