//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-03 09:45:04.
// File name: python_npc_pool.h
//
// Description:
// Define class PythonNpcPool.
//

#ifndef __GAME__SERVER__PYTHON__PYTHON__NPC__POOL__H
#define __GAME__SERVER__PYTHON__PYTHON__NPC__POOL__H

#include <boost/shared_ptr.hpp>

#include "global/smart_pool_template.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace python {

class PythonNpc;

typedef boost::shared_ptr<PythonNpc> PythonNpcPtr;

class PythonNpcPool : public global::SingletonFactory<PythonNpcPool> {
  friend class global::SingletonFactory<PythonNpcPool>;

 public:
  bool Initialize(size_t initial_number, size_t extend_number);
  void Finalize();

  PythonNpcPtr Allocate();
  void Deallocate(const PythonNpcPtr &npc);

 private:
  PythonNpcPool() {}
  ~PythonNpcPool() {}

  global::SmartPoolTemplate<PythonNpc> npcs_;
};

}  // namespace python

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__PYTHON__PYTHON__NPC__POOL__H

