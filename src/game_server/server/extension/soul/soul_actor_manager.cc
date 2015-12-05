//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-14 15:42:15.
// File name: soul_actor_manager.cc
//
// Description:
// Define class SoulActorManager.
//

#include "game_server/server/extension/soul/soul_actor_manager.h"

#include "game_server/server/extension/soul/soul_actor.h"

namespace game {

namespace server {

namespace soul {

SoulActor *SoulActorManager::Get(core::uint64 id) {
  ActorHashmap::iterator iterator = this->actors_.find(id);
  if(iterator != this->actors_.end()) {
    return iterator->second;
  }
  return NULL;
}

bool SoulActorManager::Add(SoulActor *actor) {
  // 检测指针是否合法
  if(actor == NULL) {
    return false;
  }
  // 检测是否存在
  ActorHashmap::iterator iterator = this->actors_.find(actor->GetID());
  if(iterator != this->actors_.end()) {
    return false;
  }
  // 加入管理器中
  this->actors_.insert(std::make_pair(actor->GetID(), actor));
  return true;
}

SoulActor *SoulActorManager::Remove(core::uint64 id) {
  SoulActor *actor = NULL;
  ActorHashmap::iterator iterator = this->actors_.find(id);
  if(iterator != this->actors_.end()) {
    actor = iterator->second;
    this->actors_.erase(iterator);
  }
  return actor;
}

void SoulActorManager::DailyClean() {
  ActorHashmap::iterator iterator = this->actors_.begin();
  for(; iterator != this->actors_.end(); ++iterator) {
    SoulActor *soul_actor = iterator->second;
    if(NULL == soul_actor) {
      LOG_ERROR("SoulActor is null.");
      continue;
    }
    if(soul_actor->CheckLoadFinish() == false) {
      continue;
    }
    soul_actor->DailyClean();
  }
}

}  // namespace soul

}  // namespace server

}  // namespace game

