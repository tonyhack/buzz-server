
#include "game_server/server/extension/welfare/welfare_actor_manager.h"

#include "game_server/server/extension/welfare/welfare_actor.h"

namespace game {

namespace server {

namespace welfare {

WelfareActor *WelfareActorManager::Get(core::uint64 id) {
  ActorHashmap::iterator iterator = this->actors_.find(id);
  if(iterator != this->actors_.end()) {
    return iterator->second;
  }
  return NULL;
}

bool WelfareActorManager::Add(WelfareActor *actor) {
  // 检测指针是否合法
  if(actor == NULL) {
    return false;
  }
  // 检测是否存在
  ActorHashmap::iterator iterator = this->actors_.find(actor->GetActorID());
  if(iterator != this->actors_.end()) {
    return false;
  }
  // 加入管理器中
  this->actors_.insert(std::make_pair(actor->GetActorID(), actor));
  return true;
}

WelfareActor *WelfareActorManager::Remove(core::uint64 id) {
  WelfareActor *actor = NULL;
  ActorHashmap::iterator iterator = this->actors_.find(id);
  if(iterator != this->actors_.end()) {
    actor = iterator->second;
    this->actors_.erase(iterator);
  }
  return actor;
}

void WelfareActorManager::DailyClean(core::int32 days) {
  ActorHashmap::iterator iterator = this->actors_.begin();
  for(; iterator != this->actors_.end(); ++iterator) {
    WelfareActor *actor = iterator->second;
    if(actor) {
      actor->DailyClean(days);
    }
  }
}

}  // namespace welfare

}  // namespace server

}  // namespace game

