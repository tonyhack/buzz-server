//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-03 20:26:33.
// File name: skill_event_handler.cc
//
// Description:
// Define class SkillEventHandler.
//

#include "game_server/server/extension/scene/skill_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "game_server/server/event/game_skill_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension/scene/scene_npc.h"
#include "game_server/server/extension/scene/scene_npc_manager.h"

namespace game {

namespace server {

namespace scene {

SkillEventHandler::SkillEventHandler() {}
SkillEventHandler::~SkillEventHandler() {}

bool SkillEventHandler::Initialize() {
  this->conn_npc_attacked_hit_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_SKILL_ATTACKED_HIT, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_NPC, boost::bind(&SkillEventHandler::OnEventSkillAttackedHit,
        this, _1, _2, _3, _4));

  return true;
}

void SkillEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_npc_attacked_hit_);
}

void SkillEventHandler::OnEventSkillAttackedHit(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventSkillAttackedHit) != size) {
    LOG_ERROR("参数错误");
    return;
  }

  event::EventSkillAttackedHit *event = (event::EventSkillAttackedHit *)message;

  if(event->type_ == entity::EntityType::TYPE_NPC) {
    SceneNpc *npc = SceneNpcManager::GetInstance()->Get(event->id_);
    npc->OnEvent(event::EventType::EVENT_SKILL_ATTACKED_HIT, message, size);
  }
}


}  // namespace scene

}  // namespace server

}  // namespace game

