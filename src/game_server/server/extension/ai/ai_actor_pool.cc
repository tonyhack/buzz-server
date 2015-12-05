//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-09-26 20:28:47.
// File name: ai_actor_pool.cc
//
// Description:
//
#include "game_server/server/extension/ai/ai_actor_pool.h"
#include "game_server/server/extension/ai/ai_actor.h"

namespace game {

namespace server {

namespace ai {

AiActorPool::AiActorPool() {}
AiActorPool::~AiActorPool() {}

bool AiActorPool::Initialize(size_t initial_number, size_t extend_number) {
  return this->ai_actors_.Initialize(initial_number, extend_number);
}

void AiActorPool::Finalize() {
  this->ai_actors_.Finalize();
}

AiActor *AiActorPool::Allocate() {
  return this->ai_actors_.Allocate();
}

void AiActorPool::Deallocate(AiActor *actor) {
  if(actor == NULL)
    return ;
  this->ai_actors_.Deallocate(actor);
}

} // namespace ai

} // namespace server

} // namespace game
