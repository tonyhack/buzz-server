//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-27 15:22:30.
// File name: python_actor_manager.h
//
// Description:
// Define class PythonActorManager.
//

#ifndef __GAME__SERVER__PYTHON__PYTHON__ACTOR__MANAGER__H
#define __GAME__SERVER__PYTHON__PYTHON__ACTOR__MANAGER__H

#include <ext/hash_map>

#include <boost/shared_ptr.hpp>

#include "core/base/types.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace python {

class PythonActor;
typedef boost::shared_ptr<PythonActor> PythonActorPtr;

class PythonActorManager : public global::SingletonFactory<PythonActorManager> {
  friend class global::SingletonFactory<PythonActorManager>;

  typedef __gnu_cxx::hash_map<core::uint64, PythonActorPtr> ActorHashmap;

 public:
  PythonActorPtr Get(core::uint64 id);
  bool Add(PythonActorPtr &actor);
  PythonActorPtr Remove(core::uint64 id);

 private:
  PythonActorManager() {}
  ~PythonActorManager() {}

  ActorHashmap actors_;
};

}  // namespace python

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__PYTHON__PYTHON__ACTOR__MANAGER__H

