//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-11-03 23:41:53.
// File name: configure.h
//
// Description:
//
#ifndef __GAME__SERVER__BAR__CONFIGURE__H
#define __GAME__SERVER__BAR__CONFIGURE__H

#include <map>
#include <vector>
#include <string>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace bar {

class BarCell {
 public:
  typedef std::vector<core::int32> EventMap_;

  core::int32 id_;
  core::uint64 time_;
  EventMap_ events_;
};

class Configure : public global::SingletonFactory<Configure> {
  typedef std::map<core::int32, BarCell> BarMap;

 public:
  Configure();
  ~Configure();

  bool LoadConfigure(const std::string &file);

  const BarCell *GetCell(core::int32 id) const;
  
  bool LoadBarConfigure(const std::string & file);

 private:

  core::int32 bar_init_num_;
  core::int32 bar_extend_num_;

  BarMap bars_;
};

}  // namespace bar

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__BAR__CONFIGURE__H
