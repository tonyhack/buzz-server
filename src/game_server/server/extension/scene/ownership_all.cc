#include "game_server/server/extension/scene/ownership_all.h"

#include <boost/bind.hpp>

#include "entity/entity_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension/scene/scene_npc.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace scene {

OwnershipAll::OwnershipAll() {}
OwnershipAll::~OwnershipAll() {}

bool OwnershipAll::Initialize(SceneNpc *npc) {
  if(npc == NULL || Ownership::Initialize(npc) == false) {
    LOG_ERROR("初始化失败");
    return false;
  }

  return true;
}

void OwnershipAll::Finalize() {
  Ownership::Finalize();
}

void OwnershipAll::OnEvent(event::EventType::type type,
    const void *message, size_t size) {
}

void OwnershipAll::OnEventRoleKilled(const void *message, size_t size) {
}

}  // namespace scene

}  // namespace server

}  // namespace game

