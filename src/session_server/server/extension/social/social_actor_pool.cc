#include "session_server/server/extension/social/social_actor_pool.h"

namespace session {

namespace server {

namespace social {

bool SocialActorPool::Initialize(size_t initial_number, size_t extend_number) {
  return this->actors_.Initialize(initial_number, extend_number);
}

void SocialActorPool::Finalize() {
  this->actors_.Finalize();
}

SocialActor *SocialActorPool::Allocate() {
  return this->actors_.Allocate();
}

void SocialActorPool::Deallocate(SocialActor *actor) {
  this->actors_.Deallocate(actor);
}

}  // namespace social

}  // namespace server

}  // namespace session

