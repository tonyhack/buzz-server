#include "game_server/server/extension/scene/ownership_camp_compete.h"

#include <boost/bind.hpp>

#include "entity/entity_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_role_event_types.h"
#include "game_server/server/extension/scene/scene_npc.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace scene {

OwnershipCampCompete::OwnershipCampCompete() {}
OwnershipCampCompete::~OwnershipCampCompete() {}

bool OwnershipCampCompete::Initialize(SceneNpc *npc) {
  if(npc == NULL || Ownership::Initialize(npc) == false) {
    LOG_ERROR("初始化失败");
    return false;
  }

  return true;
}

void OwnershipCampCompete::Finalize() {
  Ownership::Finalize();
}

void OwnershipCampCompete::OnEvent(event::EventType::type type,
    const void *message, size_t size) {
  switch(type) {
    case event::EventType::EVENT_ROLE_KILLED:
      this->OnEventRoleKilled(message, size);
      break;
    default:
      break;
  }
}

void OwnershipCampCompete::OnEventRoleKilled(const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventRoleKilled) != size) {
    LOG_ERROR("参数错误");
    return;
  }

  event::EventRoleKilled *event = (event::EventRoleKilled*)message;

  // 获取攻击者的阵营ID
  if(event->type_ != entity::EntityType::TYPE_NPC) {
    LOG_ERROR("类型错误");
    return ;
  }

  GameActor *game_actor = GameActorManager::GetInstance()->GetActor(
      event->attacker_id_);
  if(game_actor == NULL) {
    LOG_ERROR("玩家(%lu)没有找到", event->attacker_id_);
    return ;
  }

  this->SetValue(game_actor->GetAttribute(entity::RoleAoiFields::CAMP));
}

}  // namespace scene

}  // namespace server

}  // namespace game

