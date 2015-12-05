//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-10 13:37:45.
// File name: team_actor_manager.cc
//
// Description:
// Define class TeamActorManager.
//

#include "game_server/server/extension/team/team_actor_manager.h"

#include "game_server/server/extension/team/team_actor.h"

namespace game {

namespace server {

namespace team {

TeamActor *TeamActorManager::Get(core::uint64 id) {
  ActorHashmap::iterator iterator = this->actors_.find(id);
  if(iterator != this->actors_.end()) {
    return iterator->second;
  }
  return NULL;
}

bool TeamActorManager::Add(TeamActor *actor) {
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

TeamActor *TeamActorManager::Remove(core::uint64 id) {
  TeamActor *actor = NULL;

  ActorHashmap::iterator iterator = this->actors_.find(id);
  if(iterator != this->actors_.end()) {
    actor = iterator->second;
    this->actors_.erase(iterator);
  }

  return actor;
}

}  // namespace team

}  // namespace server

}  // namespace game

