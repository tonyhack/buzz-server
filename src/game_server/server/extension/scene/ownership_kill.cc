//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-03 17:38:21.
// File name: ownership_kill.cc //
// Description:
// Define class OwnershipKill.
//

#include "game_server/server/extension/scene/ownership_kill.h"

#include <boost/bind.hpp>

#include "entity/entity_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_role_event_types.h"
#include "game_server/server/extension/scene/scene_npc.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace scene {

OwnershipKill::OwnershipKill() {}
OwnershipKill::~OwnershipKill() {}

bool OwnershipKill::Initialize(SceneNpc *npc) {
  if(npc == NULL || Ownership::Initialize(npc) == false) {
    LOG_ERROR("初始化失败");
    return false;
  }

  return true;
}

void OwnershipKill::Finalize() {
  Ownership::Finalize();
}

void OwnershipKill::OnEvent(event::EventType::type type,
    const void *message, size_t size) {
  switch(type) {
    case event::EventType::EVENT_ROLE_KILLED:
      this->OnEventRoleKilled(message, size);
      break;
    default:
      break;
  }
}

void OwnershipKill::OnEventRoleKilled(const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventRoleKilled) != size) {
    LOG_ERROR("参数 message 或 size 错误");
    return;
  }

  event::EventRoleKilled *event = (event::EventRoleKilled*)message;

  if(event->type_ != entity::EntityType::TYPE_NPC) {
    LOG_ERROR("类型错误");
    return;
  }

  if(event->attacker_type_ == entity::EntityType::TYPE_ACTOR) {
    this->SetValue(event->attacker_id_);
  }
}

}  // namespace scene

}  // namespace server

}  // namespace game

