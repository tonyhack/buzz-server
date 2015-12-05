//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-27 18:18:06.
// File name: python_engine.h
//
// Description:
// Define class PythonEngine.
//

#ifndef __GAME__SERVER__PYTHON__PYTHON__ENGINE__H
#define __GAME__SERVER__PYTHON__PYTHON__ENGINE__H

#include <string>

#include <boost/python.hpp>
#include <boost/shared_ptr.hpp>

#include "global/logging.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace python {

class CommunicatorProxy;
typedef boost::shared_ptr<CommunicatorProxy> CommunicatorProxyPtr;

class FunctionProxy;
typedef boost::shared_ptr<FunctionProxy> FunctionProxyPtr;

class PythonEntityManager;
typedef boost::shared_ptr<PythonEntityManager> PythonEntityManagerPtr;

class RequestProxy;
typedef boost::shared_ptr<RequestProxy> RequestProxyPtr;

class LogProxy;
typedef boost::shared_ptr<LogProxy> LogProxyPtr;

class TimerProxy;
typedef boost::shared_ptr<TimerProxy> TimerProxyPtr;

class PythonEngine : public global::SingletonFactory<PythonEngine> {
  friend class global::SingletonFactory<PythonEngine>;

 public:
  PythonEngine();
  ~PythonEngine();

  bool Initialize(const std::string &module);
  void Finalize();

  boost::python::handle<> *GetModule() {
    return this->module_;
  }

 private:
  boost::python::handle<> *module_;

  CommunicatorProxyPtr communicator_proxy_;
  FunctionProxyPtr function_proxy_;
  LogProxyPtr log_proxy_;
  PythonEntityManagerPtr entity_manager_;
  RequestProxyPtr request_proxy_;
  TimerProxyPtr timer_proxy_;
};

}  // namespace python

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__PYTHON__PYTHON__ENGINE__H

