//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-03-18 18:41:02.
// File name: function_proxy.h
//
// Description:
// Define class FunctionProxy.
//

#ifndef __GAME__SERVER__PYTHON__FUNCTION__PROXY__H
#define __GAME__SERVER__PYTHON__FUNCTION__PROXY__H

#include <Python.h>

#include <string>

#include <boost/shared_ptr.hpp>

#include "core/base/types.h"

namespace game {

namespace server {

namespace python {

class FunctionProxy {
 public:
  bool SendInformMessage(core::uint64 actor, int inform_type, PyObject *params);
  bool BroadcastInformMessage(int inform_type, PyObject *params);
  const std::string GetPlayingConfigureFile() const;
};

typedef boost::shared_ptr<FunctionProxy> FunctionProxyPtr;

}  // namespace python

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__PYTHON__FUNCTION__PROXY__H

