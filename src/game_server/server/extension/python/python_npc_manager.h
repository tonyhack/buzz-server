//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-03 10:02:46.
// File name: python_npc_manager.h
//
// Description:
// Define class PythonNpcManager.
//

#ifndef __GAME__SERVER__PYTHON__PYTHON__NPC__MANAGER__H
#define __GAME__SERVER__PYTHON__PYTHON__NPC__MANAGER__H

#include <ext/hash_map>

#include <boost/shared_ptr.hpp>

#include "core/base/types.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace python {

class PythonNpc;
typedef boost::shared_ptr<PythonNpc> PythonNpcPtr;

class PythonNpcManager : public global::SingletonFactory<PythonNpcManager> {
  friend class global::SingletonFactory<PythonNpcManager>;

  typedef __gnu_cxx::hash_map<core::uint64, PythonNpcPtr> NpcHashmap;

 public:
  PythonNpcPtr Get(core::uint64 id);
  bool Add(PythonNpcPtr &npc);
  PythonNpcPtr Remove(core::uint64 id);

 private:
  PythonNpcManager() {}
  ~PythonNpcManager() {}

  NpcHashmap npcs_;
};

}  // namespace python

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__PYTHON__PYTHON__NPC__MANAGER__H

