//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-09-26 19:39:47.
// File name: ai_actor.cc
//
// Description:
//
#include "game_server/server/extension/ai/ai_actor.h"

#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

AiActor::AiActor() {}
AiActor::~AiActor() {}

bool AiActor::Initialize(GameActor *actor) {
  if(actor == NULL) {
    LOG_ERROR("初始化玩家 ai 失败");
    return false;
  }

  this->actor_ = actor;

  return true;
}

void AiActor::Finalize() {
  this->actor_ = NULL;
  this->npcs_.clear();
}

void AiActor::AddNpc(core::uint64 npc_id) {
  this->npcs_.insert(npc_id);
}

void AiActor::RemoveNpc(core::uint64 npc_id) {
  this->npcs_.erase(npc_id);
}

const AiActor::NpcSet &AiActor::GetNpcs() const {
  return this->npcs_;
}

bool AiActor::GetNpc(core::uint64 id) const {
  AiActor::NpcSet::const_iterator itr = npcs_.find(id);
  if(itr != npcs_.end()) {
    return true;
  }
  return false;
}

} // namespace ai

} // namespace server

} // namespace game

