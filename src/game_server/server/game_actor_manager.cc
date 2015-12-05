//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-02-25 18:23:55.
// File name: game_actor_manager.cc
//
// Description:
// Define class GameActorManager.
//

#include "game_server/server/game_actor_manager.h"

#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_pool.h"
#include "global/channel.h"
#include "global/logging.h"

namespace game {

namespace server {

GameActor *GameActorManager::GetActor(core::uint64 id) {
  ActorHashmap::iterator iterator = this->actors_.find(id);
  if(iterator != this->actors_.end()) {
    return iterator->second;
  }
  return NULL;
}

GameActor *GameActorManager::GetActor(const std::string &name) {
  ActorIDHashmap::iterator iterator = this->ids_.find(name);
  if(iterator == this->ids_.end()) {
    return NULL;
  }
  return this->GetActor(iterator->second);
}

bool GameActorManager::AddActor(GameActor *actor) {
  if(actor == NULL) {
    global::LogError("%s:%d (%s) Actor is null.", __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  ActorHashmap::iterator iterator1 = this->actors_.find(actor->GetActorID());
  if(iterator1 != this->actors_.end()) {
    return false;
  }
  ActorIDHashmap::iterator iterator2 = this->ids_.find(actor->GetName());
  if(iterator2 != this->ids_.end()) {
    return false;
  }
  this->actors_.insert(std::make_pair(actor->GetActorID(), actor));
  this->ids_.insert(std::make_pair(actor->GetName(), actor->GetActorID()));
  return true;
}

GameActor *GameActorManager::RemoveActor(core::uint64 id) {
  ActorHashmap::iterator iterator1 = this->actors_.find(id);
  if(iterator1 == this->actors_.end()) {
    return NULL;
  }

  GameActor *actor = iterator1->second;
  if(actor == NULL) {
    global::LogError("%s:%d (%s) Actor(%lld) is null",
        __FILE__, __LINE__, __FUNCTION__, id);
    return NULL;
  }

  ActorIDHashmap::iterator iterator2 = this->ids_.find(actor->GetName());
  if(iterator2 == this->ids_.end()) {
    global::LogError("%s:%d (%s) Actor[%lld] is not in id table.",
        __FILE__, __LINE__, __FUNCTION__, id);
    return NULL;
  }

  this->actors_.erase(iterator1);
  this->ids_.erase(iterator2);

  return actor;
}

void GameActorManager::KickActor(core::uint64 id) {
  GameActor *actor = this->GetActor(id);
  if(NULL == actor) {
    LOG_ERROR("Get GameActor(%lu) failed.", id);
    return;
  }
  actor->RequireLogout();
  this->RemoveActor(id);
  actor->Finalize();
  GameActorPool::GetInstance()->Deallocate(actor);
}

void GameActorManager::KickActors() {
  for(ActorHashmap::iterator iter = this->actors_.begin();
      iter != this->actors_.end();) {
    ActorHashmap::iterator iter2 = iter;
    ++iter2;

    GameActor *actor = iter->second;
    if(actor != NULL) {
      this->KickActor(actor->GetActorID());
    }   
    iter = iter2;
  }
}

void GameActorManager::DailyClean(core::int32 days) {
  ActorHashmap::iterator iterator = this->actors_.begin();
  for(; iterator != this->actors_.end(); ++iterator) {
    GameActor *actor = iterator->second;
    if(actor) {
      actor->DailyClean(days);
    }
  }
}

}  // namespace server

}  // namespace game

