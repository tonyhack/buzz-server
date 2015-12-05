//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-03 14:21:03.
// File name: ownership_damage.h
//
// Description:
// Define class OwnershipDamage.
//

#ifndef __GAME__SERVER__SCENE__OWNERSHIP__DAMAGE__H
#define __GAME__SERVER__SCENE__OWNERSHIP__DAMAGE__H

#include "game_server/server/extension/scene/ownership.h"

namespace game {

namespace server {

namespace scene {

class OwnershipDamage : public Ownership {
  typedef std::map<core::uint64, core::uint32> CandidateMap;

 public:
  OwnershipDamage();
  virtual ~OwnershipDamage();

  virtual bool Initialize(SceneNpc *npc);
  virtual void Finalize();

  virtual entity::NpcOwnerType::type GetType() {
    return entity::NpcOwnerType::DAMAGE;
  }

  virtual core::uint64 GetValue() const;

  virtual void OnEvent(event::EventType::type type,
      const void *message, size_t size);

 private:
  void OnEventSkillAttackedHit(const void *message, size_t size);

  CandidateMap candidates_;
};

}  // namespace scene

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SCENE__OWNERSHIP__DAMAGE__H

