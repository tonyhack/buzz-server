//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-05 18:38:04.
// File name: task_actor_manager.cc
//
// Description:
// Define class TaskActorManager.
//

#include "game_server/server/extension/task/task_actor_manager.h"

#include "game_server/server/extension/task/task_actor.h"

namespace game {

namespace server {

namespace task {

TaskActor *TaskActorManager::Get(core::uint64 id) {
  ActorHashmap::iterator iterator = this->actors_.find(id);
  if(iterator != this->actors_.end()) {
    return iterator->second;
  }
  return NULL;
}

bool TaskActorManager::Add(TaskActor *actor) {
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

TaskActor *TaskActorManager::Remove(core::uint64 id) {
  TaskActor *actor = NULL;
  ActorHashmap::iterator iterator = this->actors_.find(id);
  if(iterator != this->actors_.end()) {
    actor = iterator->second;
    this->actors_.erase(iterator);
  }
  return actor;
}

void TaskActorManager::DailyClean(core::int32 days) {
  ActorHashmap::iterator iterator = this->actors_.begin();
  for(; iterator != this->actors_.end(); ++iterator) {
    TaskActor *actor = iterator->second;
    if(actor) {
      actor->DailyClean(days);
    }
  }
}


}  // namespace task

}  // namespace server

}  // namespace game

