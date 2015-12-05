
#include "game_server/server/extension/welfare/skill_event_handler.h"

#include <boost/bind.hpp>
#include "game_server/server/event/game_skill_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension/welfare/welfare_actor.h"
#include "game_server/server/extension/welfare/welfare_actor_manager.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace welfare {

SkillEventHandler::SkillEventHandler() {}

SkillEventHandler::~SkillEventHandler() {}

bool SkillEventHandler::Initialize() {
  this->conn_skill_use_ = 
      coresh::CommunicatorSingleton::GetInstance().Follow(event::EventType::EVENT_SKILL_USE_SKILL,
      coresh::Communicator::GetChannelAll(), event::ChannelType::CHANNEL_ACTOR,
      boost::bind(&SkillEventHandler::OnEventSkillUse, this, _1, _2, _3, _4));

  return true;
}

void SkillEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_skill_use_);
}

void SkillEventHandler::OnEventSkillUse(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventUseSkill) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数 message 或 size 错误");
    return ;
  }

  event::EventUseSkill *event = (event::EventUseSkill *)message;

  // 获取玩家对象
  WelfareActor *actor = WelfareActorManager::GetInstance()->Get(event->id_);
  if(actor == NULL) {
    LOG_ERROR("玩家(%lu)没有找到", channel);
    return ;
  }

  actor->GetAchieve()->OnEvent(event::EventType::EVENT_SKILL_USE_SKILL, message, size);
}

}  // namespace welfare

}  // namespace server

}  // namespace game
