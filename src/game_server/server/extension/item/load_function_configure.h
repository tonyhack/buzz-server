//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-09-25 20:26:26.
// File name: load_function_configure.h
//
// Description:
//
#ifndef __GAME__SERVER__ITEM__LOAD__FUNCTION__CONFIGURE__H
#define __GAME__SERVER__ITEM__LOAD__FUNCTION__CONFIGURE__H

#include <ext/hash_map>

#include "entity/functionality_types.h"
#include "entity/mall_types.h"
#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace item {

class FunctionCell {
 public:
  entity::FunctionalityType::type id_;
  core::int32 dollars_;
  entity::MallBuytype::type buy_type_;
  entity::MallUseType::type spent_type_;
  core::int64 limit_start_;
  core::int64 limit_stop_;
  core::int32 freedom_dollars_;
  core::int32 time_stop_type_;
};

class LoadFunctionConfigure : public core::Noncopyable {
  typedef __gnu_cxx::hash_map<core::int32, FunctionCell> FunctionCellMap;
 
 public:
  LoadFunctionConfigure();
  ~LoadFunctionConfigure();
  
  bool LoadConfigure(const std::string &file);

  const FunctionCell *GetFunctionCell(core::int32 id) const;

 private:
  FunctionCellMap function_cells_;
};

} // namespace item

} // namespace server 

} // namespace game

#endif //__GAME__SERVER__ITEM__LOAD__FUNCTION__CONFIGURE__H

