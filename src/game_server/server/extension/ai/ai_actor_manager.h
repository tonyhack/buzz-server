//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-09-26 19:45:23.
// File name: ai_actor_manager.h
//
// Description:
//

#ifndef __GAME__SERVER__AI__AI__ACTOR__MANAGER__H
#define __GAME__SERVER__AI__AI__ACTOR__MANAGER__H

#include <ext/hash_map>

#include "core/base/types.h"
#include "game_server/server/extension/ai/ai_actor.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace ai {

class AiActorManager : public global::SingletonFactory<AiActorManager> {
  friend class global::SingletonFactory<AiActorManager>;

  typedef __gnu_cxx::hash_map<core::uint64, AiActor*> AiActorHashMap;

 public:
  AiActorManager();
  ~AiActorManager();

  bool Initialize();
  void Finalize();
  
  bool Add(AiActor *actor);
  bool Remove(core::uint64 id);

  AiActor *Get(core::uint64 id);

 private:
  // 玩家被PNC仇恨列表
  AiActorHashMap actors_;
};

} // namespace ai

} // namespace server

} // namespace game

#endif //__GAME__SERVER__AI__ACTOR__NPC__MANAGER__H

