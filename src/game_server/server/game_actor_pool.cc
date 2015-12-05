//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-02-25 18:15:00.
// File name: game_actor_pool.cc
//
// Description:
// Define class GameActorPool.
//

#include "game_server/server/game_actor_pool.h"

#include "game_server/server/game_actor.h"

namespace game {

namespace server {

bool GameActorPool::Initialize(size_t initial_number, size_t extend_number) {
  return this->actors_.Initialize(initial_number, extend_number);
}

void GameActorPool::Finalize() {
  this->actors_.Finalize();
}

GameActor *GameActorPool::Allocate() {
  GameActor *actor = this->actors_.Allocate();
  if(actor) {
    actor->Allocate();
  }
  return actor;
}

void GameActorPool::Deallocate(GameActor *actor) {
  if(actor) {
    actor->Deallocate();
    this->actors_.Deallocate(actor);
  }
}

}  // namespace server

}  // namespace game

