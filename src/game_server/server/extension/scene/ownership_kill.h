//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-03 14:13:21.
// File name: ownership_kill.h
//
// Description:
// Define class OwnershipKill.
//

#ifndef __GAME__SERVER__SCENE__OWNERSHIP__KILL__H
#define __GAME__SERVER__SCENE__OWNERSHIP__KILL__H

#include "game_server/server/extension/scene/ownership.h"

namespace game {

namespace server {

namespace scene {

class OwnershipKill : public Ownership {
 public:
  OwnershipKill();
  ~OwnershipKill();

  virtual bool Initialize(SceneNpc *npc);
  virtual void Finalize();

  virtual entity::NpcOwnerType::type GetType() {
    return entity::NpcOwnerType::KILL;
  } 

  virtual void OnEvent(event::EventType::type type,
      const void *message, size_t size);

 private:
  void OnEventRoleKilled(const void *message, size_t size);
};

}  // namespace scene

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SCENE__OWNERSHIP__KILL__H

