//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-27 15:31:18.
// File name: python_actor_manager.cc
//
// Description:
// Define class PythonActorManager.
//

#include "game_server/server/extension/python/python_actor_manager.h"

#include "game_server/server/extension/python/python_actor.h"

namespace game {

namespace server {

namespace python {

PythonActorPtr PythonActorManager::Get(core::uint64 id) {
  ActorHashmap::iterator iterator = this->actors_.find(id);
  if(iterator != this->actors_.end()) {
    return iterator->second;
  }
  return PythonActorPtr();
}

bool PythonActorManager::Add(PythonActorPtr &actor) {
  if(actor.get() == NULL) {
    return false;
  }
  ActorHashmap::iterator iterator = this->actors_.find(actor->GetID());
  if(iterator != this->actors_.end()) {
    return false;
  }
  this->actors_.insert(std::make_pair(actor->GetID(), actor));
  return true;
}

PythonActorPtr PythonActorManager::Remove(core::uint64 id) {
  PythonActorPtr actor;
  ActorHashmap::iterator iterator = this->actors_.find(id);
  if(iterator != this->actors_.end()) {
    actor = iterator->second;
    this->actors_.erase(iterator);
  }
  return actor;
}

}  // namespace python

}  // namespace server

}  // namespace game

