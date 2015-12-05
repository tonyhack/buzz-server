//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-11-03 19:58:58.
// File name: bar_actor_manager.cc
//
// Description:
//
#include "game_server/server/extension/bar/bar_actor_manager.h"

#include "game_server/server/extension/bar/bar_actor.h"

namespace game {

namespace server {

namespace bar {

BarActor *BarActorManager::Get(core::uint64 id) {
  ActorHashmap::iterator iterator = this->actors_.find(id);
  if(iterator != this->actors_.end()) {
    return iterator->second;
  }
  return NULL;
}

bool BarActorManager::Add(BarActor *actor) {
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

BarActor *BarActorManager::Remove(core::uint64 id) {
  BarActor *actor = NULL;
  ActorHashmap::iterator iterator = this->actors_.find(id);
  if(iterator != this->actors_.end()) {
    actor = iterator->second;
    this->actors_.erase(iterator);
  }
  return actor;
}

}  // namespace bar

}  // namespace server

}  // namespace game

