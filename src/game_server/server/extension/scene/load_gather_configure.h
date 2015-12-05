//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-11-08 01:05:45.
// File name: load_gather_configure.h
//
// Description:
//

#ifndef __GAME__SERVER__SCENE__LOAD__GATHER__CONFIGURE__H
#define __GAME__SERVER__SCENE__LOAD__GATHER__CONFIGURE__H

#include <ext/hash_map>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace scene {

class GatherCell {
 public:
  GatherCell() {}
  ~GatherCell() {}

  core::uint32 id_;                   // NPC
  core::int32 template_id_;           // 道具ID
  core::int32 num_;                   // 道具数目
  core::int32 buff_;                  // buff 
};

class LoadGatherConfigure : public core::Noncopyable {
  typedef __gnu_cxx::hash_map<core::uint32, GatherCell> GatherHashmap;

 public:
  LoadGatherConfigure();
  ~LoadGatherConfigure();

  bool LoadConfigure(const std::string &file);

  const GatherCell *GetCell(core::uint32 id) const;

 private:
  GatherHashmap gathers_;
};

}  // namespace scene

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SCENE__LOAD__GATHER__CONFIGURE__H

