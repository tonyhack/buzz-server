#include "session_server/server/extension/guild/guild_actor_pool.h"

namespace session {

namespace server {

namespace guild {

bool GuildActorPool::Initialize(size_t initial_number, size_t extend_number) {
  return this->actors_.Initialize(initial_number, extend_number);
}

void GuildActorPool::Finalize() {
  this->actors_.Finalize();
}

GuildActor *GuildActorPool::Allocate() {
  return this->actors_.Allocate();
}

void GuildActorPool::Deallocate(GuildActor *actor) {
  this->actors_.Deallocate(actor);
}

}  // namespace guild

}  // namespace server

}  // namespace session

