//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-27 11:36:52.
// File name: python_actor_pool.h
//
// Description:
// Define class PythonActorPool.
//

#ifndef __GAME__SERVER__PYTHON__PYTHON__ACTOR__POOL__H
#define __GAME__SERVER__PYTHON__PYTHON__ACTOR__POOL__H

#include <boost/shared_ptr.hpp>

#include "global/smart_pool_template.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace python {

class PythonActor;

typedef boost::shared_ptr<PythonActor> PythonActorPtr;

class PythonActorPool : public global::SingletonFactory<PythonActorPool> {
  friend class global::SingletonFactory<PythonActorPool>;

 public:
  bool Initialize(size_t initial_number, size_t extend_number);
  void Finalize();

  PythonActorPtr Allocate();
  void Deallocate(const PythonActorPtr &actor);

 private:
  PythonActorPool() {}
  ~PythonActorPool() {}

  global::SmartPoolTemplate<PythonActor> actors_;
};

}  // namespace python

}  // namespace server

}  // namespace game

#endif // __GAME__SERVER__PYTHON__PYTHON__ACTOR__POOL__H

