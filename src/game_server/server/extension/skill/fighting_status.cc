//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-10-17 14:09:19.
// File name: fighting_status.cc
//
// Description:
// Define class FightingStatus.
//

#include "game_server/server/extension/skill/fighting_status.h"

#include "coresh/communicator.h"
#include "entity/entity_types.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_server.h"
#include "game_server/server/global_time_tick.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_role_event_types.h"
#include "game_server/server/extension/skill/skill_role.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_skill_protocol_types.h"

namespace game {

namespace server {

namespace skill {

FightingStatus::FightingStatus() {}
FightingStatus::~FightingStatus() {}

bool FightingStatus::Initialize(SkillRole *role) {
  if(role == NULL) {
    LOG_ERROR("参数 role 错误");
    return false;
  }

  this->fighting_status_ = false;
  this->fighting_ref_count_ = 0;
  this->pvp_status_ = false;
  this->last_pvp_time_ = 0;

  this->role_ = role;

  // 玩家类型开启定时器
  if(this->role_->GetType() == entity::EntityType::TYPE_ACTOR) {
    GameServerSingleton::GetInstance().AddTimer(FightingStatus::kTimerFightingCheckID_,
        FightingStatus::kFightingCheckInterval_ * 1000, -1, this, "FightingStatus::Initialize");
  }

  return true;
}

void FightingStatus::Finalize() {
  // 玩家类型开启定时器
  if(this->role_->GetType() == entity::EntityType::TYPE_ACTOR) {
    GameServerSingleton::GetInstance().RemoveTimer(FightingStatus::kTimerFightingCheckID_, this);
  }
  this->role_ = NULL;
}

bool FightingStatus::SetStatus() {
  if(this->role_ == NULL) {
    LOG_ERROR("role_ 为 NULL");
    return false;
  }

  if(this->fighting_ref_count_++ == 0) {
    this->fighting_status_ = true;
    this->EventEnter();
  }

  return true;
}

void FightingStatus::ResetStatus(bool force) {
  if(this->role_ == NULL) {
    LOG_ERROR("role_ 为 NULL");
    return ;
  }

  if(this->fighting_status_ == false) {
    return ;
  }

  if(force == false) {
    // 使用引用计数规则退出战斗状态
    if(this->fighting_ref_count_ <= 0) {
      LOG_ERROR("战斗状态引用计数错误", this->fighting_ref_count_);
      return ;
    }
    if(--this->fighting_ref_count_ == 0) {
      this->fighting_status_ = false;
      this->EventLeave();
    }
  } else {
    // 强制退出战斗状态
    this->fighting_status_ = false;
    this->EventLeave();
    this->fighting_ref_count_ = 0;
  }
}

void FightingStatus::SetPvpFighting() {
  if(this->role_ == NULL) {
    LOG_ERROR("role_ 为 NULL");
    return ;
  }

  time_t now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
  this->SetLastPvpTime(now);
  if(this->pvp_status_ == false) {
    this->pvp_status_ = true;
    this->SetStatus();

    event::ChannelType::type channel_type = event::ChannelType::CHANNEL_MAX;
    if(this->role_->GetType() == entity::EntityType::TYPE_ACTOR) {
      channel_type = event::ChannelType::CHANNEL_ACTOR;
    } else if(this->role_->GetType() == entity::EntityType::TYPE_NPC) {
      channel_type = event::ChannelType::CHANNEL_NPC;
    } else {
      LOG_ERROR("Role类型错误");
      return ;
    }

    // 广播事件
    event::EventRoleEnterPvpStatus event;
    event.__set_type_(this->role_->GetType());
    event.__set_id_(this->role_->GetID());
    coresh::CommunicatorSingleton::GetInstance().Broadcast(
        event::EventType::EVENT_ROLE_ENTER_PVP_STATUS, this->role_->GetID(),
        channel_type, &event, sizeof(event));

    if(this->role_->GetType() == entity::EntityType::TYPE_ACTOR) {
      GameActor *actor = (GameActor*)(this->role_->GetGameRole());
      if(actor) {
        gateway::protocol::MessageEnterPvpStatus message;
        message.__set_id_(actor->GetStringID());
        actor->SendMessageAround(message,
            gateway::protocol::MessageType::MESSAGE_ENTER_PVP_STATUS, true);
      }
    }
  }
}

void FightingStatus::OnTimer(core::uint32 id) {
  if(id != FightingStatus::kTimerFightingCheckID_) {
    return ;
  }

  time_t now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();

  if(this->pvp_status_ == true &&
      this->last_pvp_time_ < now - FightingStatus::kPvpDurationSecs_) {
    this->ResetStatus();
    this->pvp_status_ = false;

    event::ChannelType::type channel_type = event::ChannelType::CHANNEL_MAX;
    if(this->role_->GetType() == entity::EntityType::TYPE_ACTOR) {
      channel_type = event::ChannelType::CHANNEL_ACTOR;
    } else if(this->role_->GetType() == entity::EntityType::TYPE_NPC) {
      channel_type = event::ChannelType::CHANNEL_NPC;
    } else {
      LOG_ERROR("Role类型错误");
      return ;
    }

    // 广播事件
    event::EventRoleLeavePvpStatus event;
    event.__set_type_(this->role_->GetType());
    event.__set_id_(this->role_->GetID());
    coresh::CommunicatorSingleton::GetInstance().Broadcast(
        event::EventType::EVENT_ROLE_LEAVE_PVP_STATUS, this->role_->GetID(),
        channel_type, &event, sizeof(event));

    if(this->role_->GetType() == entity::EntityType::TYPE_ACTOR) {
      GameActor *actor = (GameActor*)this->role_->GetGameRole();
      if(actor) {
        gateway::protocol::MessageLeavePvpStatus message;
        message.__set_id_(actor->GetStringID());
        actor->SendMessageAround(message,
            gateway::protocol::MessageType::MESSAGE_LEAVE_PVP_STATUS, true);
      }
    }
  }
}

void FightingStatus::EventEnter() {
  if(this->role_ == NULL) {
    LOG_ERROR("role_ 为 NULL");
    return ;
  }

  event::ChannelType::type channel_type = event::ChannelType::CHANNEL_MAX;
  if(this->role_->GetType() == entity::EntityType::TYPE_ACTOR) {
    channel_type = event::ChannelType::CHANNEL_ACTOR;
  } else if(this->role_->GetType() == entity::EntityType::TYPE_NPC) {
    channel_type = event::ChannelType::CHANNEL_NPC;
  } else {
    LOG_ERROR("Role类型错误");
    return ;
  }

  // 广播事件
  event::EventRoleEnterFightingStatus event;
  event.__set_type_(this->role_->GetType());
  event.__set_id_(this->role_->GetID());
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_ROLE_ENTER_FIGHTING_STATUS, this->role_->GetID(),
      channel_type, &event, sizeof(event));

  if(this->role_->GetType() == entity::EntityType::TYPE_ACTOR) {
    GameActor *actor = (GameActor*)this->role_->GetGameRole();
    if(actor) {
      gateway::protocol::MessageEnterFightingStatus message;
      actor->SendMessage(message,
          gateway::protocol::MessageType::MESSAGE_ENTER_FIGHTING_STATUS);
    }
  }

  // LOG_INFO("玩家(%lu) 进入战斗状态");
}

void FightingStatus::EventLeave() {
  if(this->role_ == NULL) {
    LOG_ERROR("role_ 为 NULL");
    return ;
  }

  event::ChannelType::type channel_type = event::ChannelType::CHANNEL_MAX;
  if(this->role_->GetType() == entity::EntityType::TYPE_ACTOR) {
    channel_type = event::ChannelType::CHANNEL_ACTOR;
  } else if(this->role_->GetType() == entity::EntityType::TYPE_NPC) {
    channel_type = event::ChannelType::CHANNEL_NPC;
  } else {
    LOG_ERROR("Role类型错误");
    return ;
  }

  // 广播事件
  event::EventRoleLeaveFightingStatus event;
  event.__set_type_(this->role_->GetType());
  event.__set_id_(this->role_->GetID());
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_ROLE_LEAVE_FIGHTING_STATUS, this->role_->GetID(),
      channel_type, &event, sizeof(event));

  if(this->role_->GetType() == entity::EntityType::TYPE_ACTOR) {
    GameActor *actor = (GameActor*)this->role_->GetGameRole();
    if(actor) {
      gateway::protocol::MessageLeaveFightingStatus message;
      actor->SendMessage(message,
          gateway::protocol::MessageType::MESSAGE_LEAVE_FIGHTING_STATUS);
    }
  }

  // LOG_INFO("玩家(%lu) 离开战斗状态");
}

}  // namespace skill

}  // namespace server

}  // namespace game

