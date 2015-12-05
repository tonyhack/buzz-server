//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-10-24 09:38:47.
// File name: timer_proxy.h
//
// Description:
// Define class TimerProxy.
//

#ifndef __GAME__SERVER__PYTHON__TIMER__PROXY__H
#define __GAME__SERVER__PYTHON__TIMER__PROXY__H

#include <string>

#include <ext/hash_map>

#include <boost/shared_ptr.hpp>

#include "core/timer_handler.h"
#include "core/base/types.h"
#include "global/hash_string.h"
#include "global/sequence_allocator.h"

namespace game {

namespace server {

namespace python {

class PythonMethodKey {
 public:
  PythonMethodKey() : id_(0) {}
  PythonMethodKey(core::uint64 id, const std::string &method)
    : id_(id), method_(method) {}

  bool operator==(const PythonMethodKey &key) const {
    return this->method_ == key.method_ &&
      this->id_ == key.id_;
  }
  bool operator<(const PythonMethodKey &key) const {
    if(this->id_ != key.id_) {
      return this->id_ < key.id_;
    } else {
      return this->method_ < key.method_;
    }
  }

  core::uint64 id_;
  std::string method_;
};

class PythonMethodHash {
 public:
  size_t operator()(const PythonMethodKey &key) const {
    return global::hash_string(key.method_.c_str()) + key.id_;
  }
};

class TimerProxy : public core::TimerHandler {
  typedef __gnu_cxx::hash_map<PythonMethodKey, core::uint32, PythonMethodHash> MethodHashmap;
  typedef __gnu_cxx::hash_map<core::uint32, PythonMethodKey> TimerHashmap;

 public:
  TimerProxy();
  virtual ~TimerProxy();

  bool Initialize();
  void Finalize();

  bool AddTimer(core::uint64 id, core::uint32 msecs, int call_times,
      const char *python_method);
  void RemoveTimer(core::uint64 id, const char *python_method);

  virtual void OnTimer(core::uint32 id);

 private:
  // Timer序列号生成器
  global::CommonSmallSequenceAllocator sequence_allocator_;

  TimerHashmap timers_;
  MethodHashmap methods_;
};

typedef boost::shared_ptr<TimerProxy> TimerProxyPtr;

}  // namespace python

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__PYTHON__TIMER__PROXY__H

