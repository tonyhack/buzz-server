//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-06-09 21:21:40.
// File name: transport_actor_manager.cc
//
// Description:
// Define class TransportActorManager.
//

#include "game_server/server/extension/transport/transport_actor_manager.h"

#include "game_server/server/extension/transport/transport_actor.h"

namespace game {

namespace server {

namespace transport {

bool TransportActorManager::Add(TransportActor *actor) {
  if (actor == NULL) {
    return false;
  }

  ActorHashmap::const_iterator iterator = this->actors_.find(actor->GetID());
  if (iterator != this->actors_.end()) {
    return false;
  }

  this->actors_.insert(std::make_pair(actor->GetID(), actor));

  return true;
}

TransportActor *TransportActorManager::Remove(core::uint64 id) {
  TransportActor *actor = NULL;

  ActorHashmap::iterator iterator = this->actors_.find(id);
  if (iterator != this->actors_.end()) {
    actor = iterator->second;
    this->actors_.erase(iterator);
  }

  return actor;
}

TransportActor *TransportActorManager::Get(core::uint64 id) {
  ActorHashmap::iterator iterator = this->actors_.find(id);
  if (iterator != this->actors_.end()) {
    return iterator->second;
  }

  return NULL;
}

bool TransportActorManager::AddTransport(core::uint64 actor, core::uint64 transport_npc) {
  TransportHashmap::const_iterator iterator = this->transports_.find(actor);
  if (iterator != this->transports_.end()) {
    return false;
  }

  this->transports_.insert(std::make_pair(actor, transport_npc));

  return true;
}

core::uint64 TransportActorManager::RemoveTransport(core::uint64 actor) {
  core::uint64 transport_npc = 0;

  TransportHashmap::iterator iterator = this->transports_.find(actor);
  if (iterator != this->transports_.end()) {
    transport_npc = iterator->second;
    this->transports_.erase(iterator);
  }

  return transport_npc;
}

core::uint64 TransportActorManager::GetTransport(core::uint64 actor) {
  TransportHashmap::iterator iterator = this->transports_.find(actor);
  if (iterator != this->transports_.end()) {
    return iterator->second;
  }

  return 0;
}

}  // namespace transport

}  // namespace server

}  // namespace game

