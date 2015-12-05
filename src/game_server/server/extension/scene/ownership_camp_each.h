#ifndef __GAME__SERVER__SCENE__OWNERSHIP__CAMP__EACH__H
#define __GAME__SERVER__SCENE__OWNERSHIP__CAMP__EACH__H

#include "game_server/server/extension/scene/ownership.h"

namespace game {

namespace server {

namespace scene {

class OwnershipCampEach : public Ownership {
  typedef std::map<core::uint64, core::uint32> CandidateMap;

 public:
  OwnershipCampEach();
  virtual ~OwnershipCampEach();

  virtual bool Initialize(SceneNpc *npc);
  virtual void Finalize();

  virtual entity::NpcOwnerType::type GetType() {
    return entity::NpcOwnerType::CAMP_EACH;
  }

  virtual void OnEvent(event::EventType::type type,
      const void *message, size_t size);

 private:
  void OnEventRoleKilled(const void *message, size_t size);
};

}  // namespace scene

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SCENE__OWNERSHIP__CAMP__EACH__H

