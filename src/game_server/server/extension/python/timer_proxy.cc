//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-10-24 11:53:36.
// File name: timer_proxy.cc
//
// Description:
// Define class TimerProxy.
//

#include "game_server/server/extension/python/timer_proxy.h"

#include <boost/bind.hpp>
#include <boost/python.hpp>

#include "game_server/server/game_server.h"
#include "game_server/server/extension/python/python_engine.h"
#include "global/logging.h"
#include "global/time_consuming.h"

namespace game {

namespace server {

namespace python {

TimerProxy::TimerProxy() {}
TimerProxy::~TimerProxy() {}

bool TimerProxy::Initialize() {
  return true;
}

void TimerProxy::Finalize() {
  TimerHashmap::iterator iterator = this->timers_.begin();
  for(; iterator != this->timers_.end(); ++iterator) {
    GameServerSingleton::GetInstance().RemoveTimer(iterator->first, this);
    this->sequence_allocator_.Deallocate(iterator->first);
  }

  this->timers_.clear();
}

bool TimerProxy::AddTimer(core::uint64 id, core::uint32 msecs, int call_times,
    const char *python_method) {
  PythonMethodKey method(id, python_method);
  MethodHashmap::const_iterator iterator2 = this->methods_.find(method);
  if(iterator2 != this->methods_.end()) {
    LOG_ERROR("timer method(%s) duplicate.", python_method);
    return false;
  }

  core::uint16 timer_id = 0;
  if(this->sequence_allocator_.Allocate(timer_id) == false) {
    LOG_ERROR("Allocate sequence number failed.");
    return false;
  }

  TimerHashmap::const_iterator iterator = this->timers_.find(timer_id);
  if(iterator != this->timers_.end()) {
    this->sequence_allocator_.Deallocate(timer_id);
    LOG_ERROR("timer id(%u) duplicate.", timer_id);
    return false;
  }

  this->timers_.insert(std::make_pair(timer_id, method));
  this->methods_.insert(std::make_pair(method, timer_id));

  GameServerSingleton::GetInstance().AddTimer(timer_id, msecs, call_times,
      this, python_method);

  return true;
}

void TimerProxy::RemoveTimer(core::uint64 id, const char *python_method) {
  PythonMethodKey method(id, python_method);
  MethodHashmap::iterator iterator = this->methods_.find(method);
  if(iterator == this->methods_.end()) {
    LOG_ERROR("Remove timer(%lu, %s) failed", id, python_method);
    return ;
  }

  GameServerSingleton::GetInstance().RemoveTimer(iterator->second, this);

  this->timers_.erase(iterator->second);
  this->methods_.erase(iterator);
}

void TimerProxy::OnTimer(core::uint32 id) {
  TimerHashmap::iterator iterator = this->timers_.find(id);
  if(iterator == this->timers_.end()) {
    return ;
  }

  // global::TimeConsuming consuming("Call timer handler:" + iterator->second.method_);

  try {

    boost::python::call_method<void>(PythonEngine::GetInstance()->GetModule()->get(),
        iterator->second.method_.c_str(), iterator->second.id_);

  } catch(boost::python::error_already_set &) {

    // TODO: 替换为异常信息写入日志
    PyErr_Print();
    LOG_ERROR("调用 python 函数(%s)失败", iterator->second.method_.c_str());

  }
}

}  // namespace python

}  // namespace server

}  // namespace game

