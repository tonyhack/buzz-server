//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-27 12:12:45.
// File name: python_actor_pool.cc
//
// Description:
// Define class PythonActorPool.
//

#include "game_server/server/extension/python/python_actor_pool.h"

#include "game_server/server/extension/python/python_actor.h"

namespace game {

namespace server {

namespace python {

bool PythonActorPool::Initialize(size_t initial_number, size_t extend_number) {
  return this->actors_.Initialize(initial_number, extend_number);
}

void PythonActorPool::Finalize() {
  this->actors_.Finalize();
}

PythonActorPtr PythonActorPool::Allocate() {
  return this->actors_.Allocate();
}

void PythonActorPool::Deallocate(const PythonActorPtr &actor) {
  this->actors_.Deallocate(actor);
}

}  // namespace python

}  // namespace server

}  // namespace game

