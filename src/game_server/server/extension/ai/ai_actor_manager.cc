//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-09-26 19:46:59.
// File name: ai_actor_manager.cc
//
// Description:
//
#include "game_server/server/extension/ai/ai_actor_manager.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

AiActorManager::AiActorManager() {}
AiActorManager::~AiActorManager() {}

bool AiActorManager::Initialize() {
   return true;
}

void AiActorManager::Finalize() {
  actors_.clear();
}

bool AiActorManager::Add(AiActor *actor) {
  if(actor == NULL) {
    LOG_ERROR("添加失败");
    return false;
  }
  AiActorHashMap::const_iterator iterator = actors_.find(actor->GetID());
  if(iterator != actors_.end()) {
    LOG_ERROR("玩家[%d]信息已经存在", actor->GetID());
    return false;
  }
  actors_.insert(std::make_pair(actor->GetID(), actor));
  return true;
}

bool AiActorManager::Remove(core::uint64 id) {
  AiActorHashMap::iterator iterator = actors_.find(id);
  if(iterator == actors_.end()) {
    LOG_ERROR("玩家(%d)没有找到", id);
    return false;
  }
  actors_.erase(iterator);
  return true;
}

AiActor *AiActorManager::Get(core::uint64 id) {
  AiActorHashMap::iterator iterator = actors_.find(id);
  if(iterator == actors_.end()) {
    return NULL;
  }
  return iterator->second;
}

} // namespace ai

} // namespace server

} // namespace game

