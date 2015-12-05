//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-09-26 19:35:54.
// File name: ai_actor.h
//
// Description:
//

#ifndef __GAME__SERVER__AI__AI__ACTOR__H
#define __GAME__SERVER__AI__AI__ACTOR__H

#include <set>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "game_server/server/game_actor.h"

namespace game {

namespace server {

class GameActor;

namespace ai {

class AiActor : public core::Noncopyable {
 public:
  typedef std::set<core::uint64> NpcSet;

  AiActor();
  ~AiActor();

  bool Initialize(GameActor *actor);
  void Finalize();
  
  inline core::uint64 GetID() const {
    if(this->actor_) {
      return this->actor_->GetID();
    }
    return 0;
  }

  // 添加、删除、查找 玩家被仇恨的NPC
  void AddNpc(core::uint64 npc_id);

  void RemoveNpc(core::uint64 npc_id);
  
  const NpcSet &GetNpcs() const;
  
  bool GetNpc(core::uint64 npc_id) const;

 private:
  GameActor *actor_;
  NpcSet npcs_;
};

} // namespace ai

} // namepsace server

} // namespace game

#endif // __GAME__SERVER__AI__AI__ACTOR__H

