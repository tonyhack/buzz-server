//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-28 12:11:55.
// File name: python_entity_manager.h
//
// Description:
// Define class PythonEntityManager.
//

#ifndef __GAME__SERVER__PYTHON__PYTHON__ENTITY__MANAGER__H
#define __GAME__SERVER__PYTHON__PYTHON__ENTITY__MANAGER__H

#include <boost/python.hpp>

#include "core/base/types.h"

namespace game {

namespace server {

namespace python {

class PythonEntityManager {
 public:
  PythonEntityManager();
  ~PythonEntityManager();

  bool GetActor(core::uint64 id);
  bool GetNpc(core::uint64 id);
};

typedef boost::shared_ptr<PythonEntityManager> PythonEntityManagerPtr;

}  // namespace python

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__PYTHON__PYTHON__ENTITY__MANAGER__H

