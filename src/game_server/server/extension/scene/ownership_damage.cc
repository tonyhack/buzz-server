//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-03 17:55:39.
// File name: ownership_damage.cc
//
// Description:
// Define class OwnershipDamage.
//

#include "game_server/server/extension/scene/ownership_damage.h"

#include <boost/bind.hpp>

#include "entity/entity_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_skill_event_types.h"
#include "game_server/server/extension/scene/scene_npc.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace scene {

OwnershipDamage::OwnershipDamage() {}
OwnershipDamage::~OwnershipDamage() {}

bool OwnershipDamage::Initialize(SceneNpc *npc) {
  if(npc == NULL || Ownership::Initialize(npc) == false) {
    LOG_ERROR("初始化失败");
    return false;
  }

  return true;
}

void OwnershipDamage::Finalize() {
  Ownership::Finalize();
  this->candidates_.clear();
}

core::uint64 OwnershipDamage::GetValue() const {
  core::uint64 owner = 0;
  core::uint64 value = 0;

  CandidateMap::const_iterator iterator = this->candidates_.begin();
  for(; iterator != this->candidates_.end(); ++iterator) {
    if(value < iterator->second) {
      owner = iterator->first;
      value = iterator->second;
    }
  }

  return owner;
}

void OwnershipDamage::OnEvent(event::EventType::type type,
    const void *message, size_t size) {
  switch(type) {
    case event::EventType::EVENT_SKILL_ATTACKED_HIT:
      this->OnEventSkillAttackedHit(message, size);
      break;
    default:
      break;
  }
}

void OwnershipDamage::OnEventSkillAttackedHit(const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventSkillAttackedHit) != size) {
    LOG_ERROR("参数错误");
    return;
  }

  event::EventSkillAttackedHit *event = (event::EventSkillAttackedHit *)message;

  if(event->type_ != entity::EntityType::TYPE_NPC ||
      (event->hurt_type_ != entity::SkillHurtType::PHYSICS_ATTACK &&
       event->hurt_type_ != entity::SkillHurtType::MAGIC_ATTACK &&
       event->hurt_type_ != entity::SkillHurtType::REDUCE_HP)) {
    return;
  }

  if(event->src_type_ == entity::EntityType::TYPE_ACTOR) {
    CandidateMap::iterator iterator = this->candidates_.find(event->src_id_);
    if(iterator != this->candidates_.end()) {
      iterator->second += event->value_;
    } else {
      this->candidates_.insert(std::make_pair(event->src_id_, event->value_));
    }
  }
}

}  // namespace scene

}  // namespace server

}  // namespace game

